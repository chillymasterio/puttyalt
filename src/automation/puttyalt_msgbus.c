/* PuttyAlt Plugin API v2 - in-process sandboxed message bus.
 * Routes typed messages between core and plugins with capability checks
 * and per-plugin rate/size limits. Self-contained C99 module. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#define MB_MAX_PLUGINS   32
#define MB_MAX_QUEUE     64
#define MB_MAX_PAYLOAD   256
#define MB_MAX_BYTES     8192   /* per-plugin published-byte budget */
#define MB_TOPIC_COUNT   4      /* number of subscribable topics      */

typedef enum {
    MSG_TOPIC_TERMINAL = 0,
    MSG_TOPIC_SESSION  = 1,
    MSG_TOPIC_NETWORK  = 2,
    MSG_TOPIC_CONTROL  = 3
} MsgTopic;

typedef struct {
    int      plugin_id;
    MsgTopic topic;
    int      len;
    char     payload[MB_MAX_PAYLOAD];
} MsgEnvelope;

typedef struct {
    uint32_t subs_mask;     /* bit per MsgTopic this plugin subscribes to */
    int32_t  bytes_used;    /* total payload bytes published so far       */
    int      registered;    /* nonzero once seen                          */
} PluginState;

typedef struct {
    PluginState plugins[MB_MAX_PLUGINS];
    MsgEnvelope queue[MB_MAX_QUEUE];
    int         head;       /* next slot to dispatch */
    int         count;      /* queued envelope count */
} MsgBus;

/* Sink receives one envelope per dispatched message; returns 0 to continue. */
typedef int (*MsgBusSink)(int plugin_id, MsgTopic topic,
                          const char *payload, int len);

static int mb_valid_plugin(int id) { return id >= 0 && id < MB_MAX_PLUGINS; }
static int mb_valid_topic(MsgTopic t) { return (int)t >= 0 && (int)t < MB_TOPIC_COUNT; }

void msgbus_init(MsgBus *b) {
    if (b == NULL) return;
    memset(b, 0, sizeof(*b));
    b->head  = 0;
    b->count = 0;
}

int msgbus_subscribe(MsgBus *b, int plugin_id, MsgTopic topic) {
    if (b == NULL || !mb_valid_plugin(plugin_id) || !mb_valid_topic(topic))
        return -1;
    PluginState *p = &b->plugins[plugin_id];
    p->registered  = 1;
    p->subs_mask  |= (uint32_t)1u << (unsigned)topic;
    return 0;
}

/* Capability + size/rate gate. Returns 0 if a publish of `len` bytes is
 * permitted for this plugin, -1 otherwise. Pure read-only check. */
int msgbus_sandbox_check(const MsgBus *b, int plugin_id, int len) {
    if (b == NULL || !mb_valid_plugin(plugin_id))
        return -1;
    if (len < 0 || len > MB_MAX_PAYLOAD)
        return -1;
    const PluginState *p = &b->plugins[plugin_id];
    if (!p->registered)
        return -1;
    if (p->bytes_used + len > MB_MAX_BYTES)
        return -1;
    if (b->count >= MB_MAX_QUEUE)
        return -1;
    return 0;
}

int msgbus_publish(MsgBus *b, int plugin_id, MsgTopic topic,
                   const char *payload, int len) {
    if (b == NULL || payload == NULL || !mb_valid_topic(topic))
        return -1;
    if (msgbus_sandbox_check(b, plugin_id, len) != 0)
        return -1;
    /* Capability check: publisher must hold a subscription to the topic. */
    PluginState *p = &b->plugins[plugin_id];
    if ((p->subs_mask & ((uint32_t)1u << (unsigned)topic)) == 0)
        return -1;

    int tail = (b->head + b->count) % MB_MAX_QUEUE;
    MsgEnvelope *e = &b->queue[tail];
    e->plugin_id = plugin_id;
    e->topic     = topic;
    e->len       = len;
    memcpy(e->payload, payload, (size_t)len);
    if (len < MB_MAX_PAYLOAD)
        e->payload[len] = '\0';
    else
        e->payload[MB_MAX_PAYLOAD - 1] = '\0';

    p->bytes_used += len;
    b->count++;
    return 0;
}

/* Drain queued envelopes to the sink. Returns count dispatched, or -1. */
int msgbus_dispatch(MsgBus *b, MsgBusSink sink) {
    if (b == NULL || sink == NULL)
        return -1;
    int dispatched = 0;
    while (b->count > 0) {
        MsgEnvelope *e = &b->queue[b->head];
        char view[MB_MAX_PAYLOAD];
        int n = e->len;
        if (n < 0) n = 0;
        if (n > MB_MAX_PAYLOAD - 1) n = MB_MAX_PAYLOAD - 1;
        snprintf(view, sizeof(view), "%.*s", n, e->payload);
        int rc = sink(e->plugin_id, e->topic, view, e->len);
        b->head = (b->head + 1) % MB_MAX_QUEUE;
        b->count--;
        dispatched++;
        if (rc != 0)
            break;   /* sink requested stop */
    }
    return dispatched;
}
