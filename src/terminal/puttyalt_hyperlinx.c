/* puttyalt_hyperlinx.c - OSC 8 hyperlink parser for PuttyAlt.
 * Parses OSC 8 ; params ; URI ST: extracts id= and the target URI,
 * tracks the open link, and matches same-link cells. C99, no deps. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define HLX_URI_MAX 512
#define HLX_ID_MAX  128

typedef struct {
    int      open;            /* nonzero while a hyperlink is active */
    uint32_t serial;          /* unique per opened link, 0 = none    */
    uint64_t opened_ms;       /* timestamp the link was opened        */
    char     uri[HLX_URI_MAX];
    char     id[HLX_ID_MAX];
} hyperlinx_state;

void hyperlinx_init(hyperlinx_state *s)
{
    if (!s) return;
    memset(s, 0, sizeof(*s));
}

/* Pull the id= field out of a ':'-separated params blob into id[idsz]. */
static void hlx_extract_id(const char *params, char *id, size_t idsz)
{
    const char *p = params;
    id[0] = '\0';
    while (p && *p) {
        const char *end = strchr(p, ':');
        size_t seg = end ? (size_t)(end - p) : strlen(p);
        if (seg > 3 && strncmp(p, "id=", 3) == 0) {
            size_t vlen = seg - 3;
            if (vlen >= idsz) vlen = idsz - 1;
            memcpy(id, p + 3, vlen);
            id[vlen] = '\0';
            return;
        }
        if (!end) break;
        p = end + 1;
    }
}

/* Parse one OSC 8 payload: the text after "8;" (params ; uri).
 * Empty params and empty uri (8;;) closes the current link.
 * Returns 0 on success, -1 on bad input. */
int hyperlinx_parse(hyperlinx_state *s, const char *payload, uint64_t now_ms)
{
    const char *sep, *uri;
    char params[HLX_ID_MAX + 16];
    size_t plen;

    if (!s || !payload) return -1;

    sep = strchr(payload, ';');
    if (!sep) return -1;
    /* OSC 8 needs params ; uri */
    uri = sep + 1;

    plen = (size_t)(sep - payload);
    if (plen >= sizeof(params)) plen = sizeof(params) - 1;
    memcpy(params, payload, plen);
    params[plen] = '\0';

    if (uri[0] == '\0') {            /* close marker: 8;<params>; */
        s->open = 0; s->id[0] = '\0'; s->uri[0] = '\0';
        return 0;
    }

    if (strlen(uri) >= HLX_URI_MAX) return -1;
    strncpy(s->uri, uri, HLX_URI_MAX - 1);
    s->uri[HLX_URI_MAX - 1] = '\0';
    hlx_extract_id(params, s->id, sizeof(s->id));

    s->open = 1;
    s->opened_ms = now_ms;
    if (++s->serial == 0) s->serial = 1;   /* never reuse 0 */
    return 0;
}

int hyperlinx_current(const hyperlinx_state *s)
{
    return (s && s->open) ? (int)s->serial : 0;
}

const char *hyperlinx_uri(const hyperlinx_state *s)
{
    return (s && s->open) ? s->uri : NULL;
}

const char *hyperlinx_id(const hyperlinx_state *s)
{
    return (s && s->open && s->id[0]) ? s->id : NULL;
}

/* Two cells share a logical hyperlink when both carry the same nonzero
 * serial, or both share the same explicit id= string. */
int hyperlinx_same(const hyperlinx_state *s, uint32_t a_serial,
                   uint32_t b_serial, const char *a_id, const char *b_id)
{
    if (!s) return 0;
    if (a_id && b_id && a_id[0] && b_id[0])
        return strcmp(a_id, b_id) == 0;
    return a_serial != 0 && a_serial == b_serial;
}

int hyperlinx_count(const hyperlinx_state *s)
{
    return s ? (int)s->serial : 0;
}
