/* PuttyAlt Plugin API v2 -- hook dispatch with sandbox.
 * Registers plugin callbacks against named hook points and dispatches core
 * events to enabled, capability-granted plugins in priority order. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#define HOOKD_MAX_HANDLERS 64
#define HOOKD_DESC_LEN     96

typedef enum {
    HOOK_SESSION_OPEN = 0, HOOK_SESSION_CLOSE, HOOK_DATA_RX,
    HOOK_DATA_TX, HOOK_KEYPRESS, HOOK_POINT_COUNT /* sentinel */
} HookPoint;

typedef struct {
    HookPoint   point;
    int         plugin_id, payload_len;
    uint64_t    now_ms;        /* caller-supplied clock */
    uint32_t    caps_required; /* capability bitmask this event needs */
    const char *payload;       /* may be NULL */
} HookEvent;

typedef int (*HookFn)(const HookEvent *ev, void *user);

typedef struct {
    int       used, enabled, plugin_id, priority; /* higher priority fires first */
    HookPoint point;
    HookFn    fn;
    uint32_t  caps_granted;    /* sandbox: capabilities this plugin holds */
} HookHandler;

typedef struct {
    HookHandler slots[HOOKD_MAX_HANDLERS];
    int         count;
    char        last_error[HOOKD_DESC_LEN];
} HookTable;

void hookd_init(HookTable *t)
{
    if (!t) return;
    memset(t, 0, sizeof(*t));
    snprintf(t->last_error, sizeof(t->last_error), "ok");
}

int hookd_register(HookTable *t, int plugin_id, HookPoint pt, HookFn fn, int priority)
{
    int i, free_slot = -1;
    if (!t || !fn || plugin_id < 0 || pt < 0 || pt >= HOOK_POINT_COUNT) return -1;
    for (i = 0; i < HOOKD_MAX_HANDLERS; i++) {
        if (!t->slots[i].used) { if (free_slot < 0) free_slot = i; continue; }
        if (t->slots[i].plugin_id == plugin_id && t->slots[i].point == pt) {
            snprintf(t->last_error, sizeof(t->last_error),
                     "duplicate plugin %d on hook %d", plugin_id, (int)pt);
            return -1;
        }
    }
    if (free_slot < 0) {
        snprintf(t->last_error, sizeof(t->last_error), "table full (%d)",
                 HOOKD_MAX_HANDLERS);
        return -1;
    }
    memset(&t->slots[free_slot], 0, sizeof(t->slots[free_slot]));
    t->slots[free_slot].used = t->slots[free_slot].enabled = 1;
    t->slots[free_slot].plugin_id = plugin_id;
    t->slots[free_slot].point = pt;
    t->slots[free_slot].fn = fn;
    t->slots[free_slot].priority = priority;
    t->slots[free_slot].caps_granted = 0xFFFFFFFFu; /* default full sandbox grant */
    t->count++;
    return 0;
}

int hookd_unregister(HookTable *t, int plugin_id, HookPoint pt)
{
    int i, removed = 0;
    if (!t || plugin_id < 0 || pt < 0 || pt >= HOOK_POINT_COUNT) return -1;
    for (i = 0; i < HOOKD_MAX_HANDLERS; i++)
        if (t->slots[i].used && t->slots[i].plugin_id == plugin_id &&
            t->slots[i].point == pt) {
            memset(&t->slots[i], 0, sizeof(t->slots[i]));
            t->count--; removed++;
        }
    return removed ? 0 : -1;
}

int hookd_fire(HookTable *t, HookPoint pt, const HookEvent *ev)
{
    int i, fired = 0, best;
    uint32_t done[HOOKD_MAX_HANDLERS / 32 + 1];
    if (!t || !ev || pt < 0 || pt >= HOOK_POINT_COUNT) return -1;
    memset(done, 0, sizeof(done));
    for (;;) { /* select highest-priority undispatched handler each pass */
        best = -1;
        for (i = 0; i < HOOKD_MAX_HANDLERS; i++) {
            if (!t->slots[i].used || t->slots[i].point != pt) continue;
            if (done[i / 32] & (1u << (i % 32))) continue;
            if (best < 0 || t->slots[i].priority > t->slots[best].priority) best = i;
        }
        if (best < 0) break;
        done[best / 32] |= (1u << (best % 32));
        if (!t->slots[best].enabled) continue;
        /* sandbox check: every required capability must be granted */
        if ((ev->caps_required & t->slots[best].caps_granted) != ev->caps_required)
            continue;
        if (t->slots[best].fn(ev, NULL) == 0) fired++;
    }
    return fired;
}

int hookd_count(const HookTable *t, HookPoint pt)
{
    int i, n = 0;
    if (!t || pt < 0 || pt >= HOOK_POINT_COUNT) return -1;
    for (i = 0; i < HOOKD_MAX_HANDLERS; i++)
        if (t->slots[i].used && t->slots[i].point == pt) n++;
    return n;
}
