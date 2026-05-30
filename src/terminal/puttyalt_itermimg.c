/* puttyalt_itermimg.c - iTerm2 OSC 1337 File= inline image protocol parser. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define ITERMIMG_MAX_ARGS   16
#define ITERMIMG_KEY_LEN    32
#define ITERMIMG_VAL_LEN    64

typedef struct ItermImgArg {
    char key[ITERMIMG_KEY_LEN];
    char val[ITERMIMG_VAL_LEN];
} ItermImgArg;

typedef struct ItermImgDesc {
    ItermImgArg args[ITERMIMG_MAX_ARGS];
    int         arg_count;
    int         payload_off;   /* byte offset of base64 payload within data */
    int         payload_len;   /* length of base64 payload */
    int         valid;
} ItermImgDesc;

typedef struct ItermImgState {
    uint32_t parsed_count;
    uint32_t error_count;
    int      last_payload_len;
} ItermImgState;

/* Prefix that introduces the sequence: ESC ] 1337 ; File = */
static const char ITERMIMG_PREFIX[] = "\033]1337;File=";
#define ITERMIMG_PREFIX_LEN ((int)(sizeof(ITERMIMG_PREFIX) - 1))

void itermimg_init(ItermImgState *st)
{
    if (!st) return;
    st->parsed_count = 0;
    st->error_count = 0;
    st->last_payload_len = 0;
}

void itermimg_reset(ItermImgState *st)
{
    if (!st) return;
    st->last_payload_len = 0;
}

/* Return 1 if data begins with the iTerm2 File= prefix, else 0. */
int itermimg_match(const unsigned char *data, int len)
{
    if (!data || len < ITERMIMG_PREFIX_LEN) return 0;
    return memcmp(data, ITERMIMG_PREFIX, ITERMIMG_PREFIX_LEN) == 0 ? 1 : 0;
}

/* Parse args (key=value pairs separated by ';') ending at ':' which
 * precedes the base64 payload. Payload runs to BEL or ST or end of buffer. */
int itermimg_parse(ItermImgState *st, const unsigned char *data, int len,
                   ItermImgDesc *out)
{
    int i, j;
    if (!data || !out) return -1;
    memset(out, 0, sizeof(*out));
    if (!itermimg_match(data, len)) {
        if (st) st->error_count++;
        return -1;
    }
    i = ITERMIMG_PREFIX_LEN;
    /* Parse the key=value list up to the ':' separator. */
    while (i < len && data[i] != ':' && data[i] != '\007' && data[i] != '\033') {
        ItermImgArg *a;
        if (out->arg_count >= ITERMIMG_MAX_ARGS) { if (st) st->error_count++; return -1; }
        a = &out->args[out->arg_count];
        for (j = 0; i < len && data[i] != '=' && data[i] != ';' && data[i] != ':'; i++)
            if (j < ITERMIMG_KEY_LEN - 1) a->key[j++] = (char)data[i];
        a->key[j] = '\0';
        if (i < len && data[i] == '=') {
            i++;
            for (j = 0; i < len && data[i] != ';' && data[i] != ':' && data[i] != '\007'
                        && data[i] != '\033'; i++)
                if (j < ITERMIMG_VAL_LEN - 1) a->val[j++] = (char)data[i];
            a->val[j] = '\0';
        }
        if (a->key[0] != '\0') out->arg_count++;
        if (i < len && data[i] == ';') i++;
    }
    if (i >= len || data[i] != ':') { if (st) st->error_count++; return -1; }
    i++; /* skip ':' */
    out->payload_off = i;
    for (j = i; j < len && data[j] != '\007' && data[j] != '\033'; j++)
        ;
    out->payload_len = j - i;
    out->valid = 1;
    if (st) { st->parsed_count++; st->last_payload_len = out->payload_len; }
    return 0;
}

/* Look up an integer-valued argument; return fallback if missing/non-numeric. */
int itermimg_arg_int(const ItermImgDesc *desc, const char *key, int fallback)
{
    int k;
    char *end;
    long v;
    if (!desc || !key) return fallback;
    for (k = 0; k < desc->arg_count; k++) {
        if (strcmp(desc->args[k].key, key) == 0) {
            if (desc->args[k].val[0] == '\0') return fallback;
            v = strtol(desc->args[k].val, &end, 10);
            if (end == desc->args[k].val) return fallback;
            return (int)v;
        }
    }
    return fallback;
}

/* Report the payload byte span; returns 0 on success, -1 if no valid payload. */
int itermimg_payload_span(const ItermImgDesc *desc, int *off, int *len)
{
    if (!desc || !desc->valid || desc->payload_len < 0) return -1;
    if (off) *off = desc->payload_off;
    if (len) *len = desc->payload_len;
    return 0;
}
