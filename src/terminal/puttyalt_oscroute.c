/* puttyalt_oscroute.c - OSC/APC control sequence router for PuttyAlt.
 * Accumulates OSC/APC payload bytes and dispatches complete sequences to
 * registered graphics handlers keyed by a leading numeric or letter code,
 * keeping image-protocol detection out of the main escape parser. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#define OSCROUTE_MAX_HANDLERS 16
#define OSCROUTE_BUF_SIZE     4096

/* Handler: gets the payload (minus the leading code prefix) and user ptr. */
typedef int (*OscHandlerFn)(int code, const unsigned char *seq, int len,
                            void *user);

typedef struct OscHandler {
    int          code;   /* numeric (>=0) or letter (negated ASCII) key */
    OscHandlerFn fn;
    void        *user;
    int          used;
} OscHandler;

typedef struct OscRouter {
    OscHandler    handlers[OSCROUTE_MAX_HANDLERS];
    int           nhandlers;
    unsigned char buf[OSCROUTE_BUF_SIZE];
    int           len;        /* bytes currently buffered */
    int           overflow;   /* set when payload exceeded the buffer */
    char          lastlabel[24]; /* label of most recent dispatched code */
} OscRouter;

/* Render a human-readable label for a code into out (for logging/debug). */
static void oscroute_label(int code, char *out, int outlen)
{
    if (!out || outlen <= 0)
        return;
    if (code < 0)
        snprintf(out, (size_t)outlen, "APC '%c'", (char)(-code));
    else
        snprintf(out, (size_t)outlen, "OSC %d", code);
}

void oscroute_init(OscRouter *r)
{
    if (!r)
        return;
    r->lastlabel[0] = '\0';
    memset(r->handlers, 0, sizeof r->handlers);
    r->nhandlers = 0;
    r->len = 0;
    r->overflow = 0;
}

int oscroute_register(OscRouter *r, int code, OscHandlerFn fn, void *user)
{
    int i;
    if (!r || !fn)
        return -1;
    /* Replace an existing registration for the same code. */
    for (i = 0; i < OSCROUTE_MAX_HANDLERS; i++) {
        if (r->handlers[i].used && r->handlers[i].code == code) {
            r->handlers[i].fn = fn;
            r->handlers[i].user = user;
            return 0;
        }
    }
    for (i = 0; i < OSCROUTE_MAX_HANDLERS; i++) {
        if (!r->handlers[i].used) {
            r->handlers[i].code = code;
            r->handlers[i].fn = fn;
            r->handlers[i].user = user;
            r->handlers[i].used = 1;
            r->nhandlers++;
            return 0;
        }
    }
    return -1; /* table full */
}

int oscroute_feed(OscRouter *r, const unsigned char *data, int len)
{
    int i;
    if (!r || !data || len < 0)
        return -1;
    for (i = 0; i < len; i++) {
        if (r->len >= OSCROUTE_BUF_SIZE) {
            r->overflow = 1;
            return -1;
        }
        r->buf[r->len++] = data[i];
    }
    return 0;
}

int oscroute_dispatch(OscRouter *r, const unsigned char *seq, int len)
{
    int i = 0, code, h;
    if (!r || !seq || len < 0)
        return -1;

    /* Parse a leading numeric code (e.g. "1337;...") or a single letter
     * code (e.g. APC 'G' for Kitty graphics). */
    if (i < len && seq[i] >= '0' && seq[i] <= '9') {
        code = 0;
        while (i < len && seq[i] >= '0' && seq[i] <= '9') {
            code = code * 10 + (seq[i] - '0');
            if (code > 1000000) /* clamp to keep within sane bounds */
                code = 1000000;
            i++;
        }
        if (i < len && seq[i] == ';')
            i++; /* skip separator */
    } else if (i < len &&
               ((seq[i] >= 'A' && seq[i] <= 'Z') ||
                (seq[i] >= 'a' && seq[i] <= 'z'))) {
        code = -(int)seq[i]; /* letter codes stored as negated ASCII */
        i++;
    } else {
        return -1; /* no recognizable code */
    }

    oscroute_label(code, r->lastlabel, (int)sizeof r->lastlabel);

    for (h = 0; h < OSCROUTE_MAX_HANDLERS; h++) {
        if (r->handlers[h].used && r->handlers[h].code == code)
            return r->handlers[h].fn(code, seq + i, len - i,
                                     r->handlers[h].user);
    }
    return -1; /* no handler registered for this code */
}

void oscroute_reset(OscRouter *r)
{
    if (!r)
        return;
    r->len = 0;
    r->overflow = 0;
}
