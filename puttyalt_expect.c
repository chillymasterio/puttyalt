#include "puttyalt_expect.h"
#include <string.h>
#include <stdio.h>

void expect_init(ExpectEngine *e)
{
    memset(e, 0, sizeof(*e));
    e->global_timeout_ms = 30000;
}

int expect_add_rule(ExpectEngine *e, const char *pattern,
                    const char *response, ExpectAction action)
{
    if (e->count >= EXPECT_MAX_RULES) return -1;
    if (!pattern || pattern[0] == '\0') return -1;

    ExpectRule *r = &e->rules[e->count];
    memset(r, 0, sizeof(*r));
    snprintf(r->pattern, sizeof(r->pattern), "%s", pattern);
    if (response)
        snprintf(r->response, sizeof(r->response), "%s", response);
    r->action = action;
    r->timeout_ms = e->global_timeout_ms;
    r->case_sensitive = 1;
    e->count++;
    return 0;
}

static const char *find_pattern(const char *haystack, int hlen,
                                const char *needle, int case_sens)
{
    int nlen = (int)strlen(needle);
    if (nlen > hlen) return NULL;

    for (int i = 0; i <= hlen - nlen; i++) {
        int match = 1;
        for (int j = 0; j < nlen; j++) {
            char a = haystack[i + j];
            char b = needle[j];
            if (!case_sens) {
                if (a >= 'A' && a <= 'Z') a += 32;
                if (b >= 'A' && b <= 'Z') b += 32;
            }
            if (a != b) { match = 0; break; }
        }
        if (match) return &haystack[i];
    }
    return NULL;
}

int expect_feed(ExpectEngine *e, const char *data, int len,
                char *send_out, int max_out)
{
    if (!e->active || len <= 0) return 0;

    /* Append to buffer */
    int space = (int)sizeof(e->buffer) - e->buf_len - 1;
    if (len > space) len = space;
    memcpy(e->buffer + e->buf_len, data, len);
    e->buf_len += len;
    e->buffer[e->buf_len] = '\0';

    send_out[0] = '\0';
    int out_len = 0;

    for (int i = 0; i < e->count; i++) {
        ExpectRule *r = &e->rules[i];
        if (r->once && r->fired) continue;

        if (find_pattern(e->buffer, e->buf_len, r->pattern, r->case_sensitive)) {
            r->fired = 1;

            switch (r->action) {
            case EXPECT_ACT_SEND:
                out_len = snprintf(send_out, max_out, "%s", r->response);
                break;
            case EXPECT_ACT_SEND_LINE:
                out_len = snprintf(send_out, max_out, "%s\r\n", r->response);
                break;
            case EXPECT_ACT_WAIT:
                break;
            case EXPECT_ACT_LOG:
                break;
            case EXPECT_ACT_BREAK:
                e->active = 0;
                break;
            }

            /* Clear buffer after match */
            e->buf_len = 0;
            e->buffer[0] = '\0';
            return out_len;
        }
    }

    /* Trim buffer if too large */
    if (e->buf_len > 2048) {
        memmove(e->buffer, e->buffer + e->buf_len - 1024, 1024);
        e->buf_len = 1024;
        e->buffer[e->buf_len] = '\0';
    }

    return 0;
}

void expect_reset(ExpectEngine *e)
{
    for (int i = 0; i < e->count; i++)
        e->rules[i].fired = 0;
    e->buf_len = 0;
    e->buffer[0] = '\0';
    e->current_rule = 0;
}

void expect_start(ExpectEngine *e) { e->active = 1; expect_reset(e); }
void expect_stop(ExpectEngine *e) { e->active = 0; }
