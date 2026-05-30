/* PuttyAlt: Kitty graphics protocol APC _G control-sequence parser.
 * Parses ESC _ G <k=v,k=v,...> ; <base64 payload> ESC \
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define KITTYGFX_MAX_KEYS    32
#define KITTYGFX_MAX_PAYLOAD 4096

typedef struct {
    char  key;   /* single-letter control key, e.g. 'a','f','m','i' */
    char  val[16];
} KittyGfxKey;

typedef struct {
    KittyGfxKey   keys[KITTYGFX_MAX_KEYS];
    int           key_count;
    char          payload[KITTYGFX_MAX_PAYLOAD];
    int           payload_len;
} KittyGfxCmd;

typedef struct {
    int   parsed_count;   /* commands successfully parsed */
    int   error_count;    /* malformed sequences seen */
} KittyGfxState;

void kittygfx_init(KittyGfxState *st) {
    if (!st) return;
    st->parsed_count = 0;
    st->error_count  = 0;
}

/* Returns offset of APC _G start (ESC _ G), or -1 if not present. */
int kittygfx_match(const unsigned char *data, int len) {
    if (!data || len < 3) return -1;
    for (int i = 0; i + 2 < len; i++)
        if (data[i] == 0x1b && data[i + 1] == '_' && data[i + 2] == 'G')
            return i;
    return -1;
}

/* Parse one "k=value" pair (length n) into cmd. */
static int kittygfx_add_pair(KittyGfxCmd *cmd, const char *s, int n) {
    if (cmd->key_count >= KITTYGFX_MAX_KEYS || n < 1) return -1;
    KittyGfxKey *k = &cmd->keys[cmd->key_count];
    k->key = s[0];
    int vi = 0;
    if (n >= 2 && s[1] == '=') {
        for (int j = 2; j < n && vi < (int)sizeof(k->val) - 1; j++)
            k->val[vi++] = s[j];
    }
    k->val[vi] = '\0';
    cmd->key_count++;
    return 0;
}

int kittygfx_parse(KittyGfxState *st, const unsigned char *data, int len,
                   KittyGfxCmd *out) {
    if (!st || !data || !out) return -1;
    int start = kittygfx_match(data, len);
    if (start < 0) { st->error_count++; return -1; }
    memset(out, 0, sizeof(*out));
    int i = start + 3;                 /* skip ESC _ G */
    int seg = i;
    /* control keys up to ';' or terminator */
    while (i < len && data[i] != ';' && data[i] != 0x1b) {
        if (data[i] == ',') {
            if (kittygfx_add_pair(out, (const char *)&data[seg], i - seg) < 0)
                { st->error_count++; return -1; }
            seg = i + 1;
        }
        i++;
    }
    if (i > seg)
        if (kittygfx_add_pair(out, (const char *)&data[seg], i - seg) < 0)
            { st->error_count++; return -1; }
    /* payload after ';' until ESC */
    if (i < len && data[i] == ';') {
        i++;
        while (i < len && data[i] != 0x1b &&
               out->payload_len < KITTYGFX_MAX_PAYLOAD - 1)
            out->payload[out->payload_len++] = (char)data[i++];
        out->payload[out->payload_len] = '\0';
    }
    /* require ST terminator: ESC \ */
    if (!(i + 1 < len && data[i] == 0x1b && data[i + 1] == '\\')) {
        st->error_count++;
        return -1;
    }
    st->parsed_count++;
    return 0;
}

/* Returns the action code ('t','T','q','p','d','a',...); 't' is the default. */
char kittygfx_action(const KittyGfxCmd *cmd) {
    if (!cmd) return '\0';
    for (int i = 0; i < cmd->key_count; i++)
        if (cmd->keys[i].key == 'a' && cmd->keys[i].val[0])
            return cmd->keys[i].val[0];
    return 't';
}

/* Integer value of a control key, or fallback if absent/non-numeric. */
int kittygfx_key_int(const KittyGfxCmd *cmd, char key, int fallback) {
    if (!cmd) return fallback;
    for (int i = 0; i < cmd->key_count; i++) {
        if (cmd->keys[i].key == key) {
            const char *v = cmd->keys[i].val;
            if (!v[0]) return fallback;
            char *end = NULL;
            long n = strtol(v, &end, 10);
            if (end == v) return fallback;
            return (int)n;
        }
    }
    return fallback;
}

/* True when more chunks follow (m=1). */
int kittygfx_is_chunk_more(const KittyGfxCmd *cmd) {
    return kittygfx_key_int(cmd, 'm', 0) == 1 ? 1 : 0;
}
