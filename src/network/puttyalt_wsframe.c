/* puttyalt_wsframe.c - RFC6455 WebSocket frame codec for SSH-over-WebSocket. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define WSFRAME_MAX_PAYLOAD 65536u
#define WSFRAME_MAX_HEADER  14u

enum wsframe_opcode {
    WSFRAME_OP_CONT = 0x0, WSFRAME_OP_TEXT = 0x1, WSFRAME_OP_BIN = 0x2,
    WSFRAME_OP_CLOSE = 0x8, WSFRAME_OP_PING = 0x9, WSFRAME_OP_PONG = 0xA
};

enum wsframe_state { WSFRAME_ST_HDR = 0, WSFRAME_ST_LEN, WSFRAME_ST_MASK, WSFRAME_ST_DATA, WSFRAME_ST_DONE };

enum wsframe_err { WSFRAME_OK = 0, WSFRAME_E_SHORT = -1, WSFRAME_E_TOOBIG = -2, WSFRAME_E_RANGE = -3 };

typedef struct wsframe_hdr {
    int fin, mask;
    uint8_t opcode;
    uint64_t payload_len;
    uint8_t mask_key[4];
    size_t header_len;
} wsframe_hdr;

typedef struct wsframe_ctx {
    enum wsframe_state state;
    uint8_t mask_key[4];
    wsframe_hdr hdr;
    int last_err;
} wsframe_ctx;

int wsframe_init(wsframe_ctx *c, const uint8_t key[4]) {
    if (!c) return -1;
    memset(c, 0, sizeof(*c));
    c->state = WSFRAME_ST_HDR;
    if (key) memcpy(c->mask_key, key, 4);
    else { c->mask_key[0] = 0x3a; c->mask_key[1] = 0x7f; c->mask_key[2] = 0xc1; c->mask_key[3] = 0x05; }
    return 0;
}

void wsframe_reset(wsframe_ctx *c) {
    if (!c) return;
    c->state = WSFRAME_ST_HDR;
    memset(&c->hdr, 0, sizeof(c->hdr));
    c->last_err = WSFRAME_OK;
}

void wsframe_apply_mask(uint8_t *data, size_t len, const uint8_t key[4]) {
    size_t i;
    if (!data || !key) return;
    for (i = 0; i < len; i++) data[i] ^= key[i & 3u];
}

const char *wsframe_opcode_name(uint8_t op) {
    switch (op) {
        case WSFRAME_OP_CONT: return "CONT";
        case WSFRAME_OP_TEXT: return "TEXT";
        case WSFRAME_OP_BIN:  return "BIN";
        case WSFRAME_OP_CLOSE:return "CLOSE";
        case WSFRAME_OP_PING: return "PING";
        case WSFRAME_OP_PONG: return "PONG";
        default: return "UNKNOWN";
    }
}

const char *wsframe_strerror(int err) {
    switch (err) {
        case WSFRAME_OK:       return "ok";
        case WSFRAME_E_SHORT:  return "short buffer";
        case WSFRAME_E_TOOBIG: return "payload too large";
        case WSFRAME_E_RANGE:  return "value out of range";
        default:               return "unknown error";
    }
}

uint64_t wsframe_payload_len(const wsframe_hdr *h) { return h ? h->payload_len : 0; }

int wsframe_parse_header(const uint8_t *buf, size_t len, wsframe_hdr *h) {
    size_t pos = 2, i;
    uint8_t l7;
    if (!buf || !h || len < 2) return WSFRAME_E_SHORT;
    memset(h, 0, sizeof(*h));
    h->fin = (buf[0] & 0x80) ? 1 : 0;
    h->opcode = (uint8_t)(buf[0] & 0x0F);
    h->mask = (buf[1] & 0x80) ? 1 : 0;
    l7 = (uint8_t)(buf[1] & 0x7F);
    if (l7 < 126) {
        h->payload_len = l7;
    } else if (l7 == 126) {
        if (len < pos + 2) return WSFRAME_E_SHORT;
        h->payload_len = ((uint64_t)buf[pos] << 8) | buf[pos + 1];
        pos += 2;
    } else {
        if (len < pos + 8) return WSFRAME_E_SHORT;
        h->payload_len = 0;
        for (i = 0; i < 8; i++) h->payload_len = (h->payload_len << 8) | buf[pos + i];
        pos += 8;
    }
    if (h->mask) {
        if (len < pos + 4) return WSFRAME_E_SHORT;
        memcpy(h->mask_key, buf + pos, 4);
        pos += 4;
    }
    if (h->payload_len > WSFRAME_MAX_PAYLOAD) return WSFRAME_E_TOOBIG;
    h->header_len = pos;
    return WSFRAME_OK;
}

int wsframe_encode(wsframe_ctx *c, uint8_t opcode, const uint8_t *payload, size_t plen,
                   uint8_t *out, size_t out_cap, size_t *out_len) {
    size_t pos = 0, i;
    if (!c || !out || !out_len || (plen && !payload)) return -1;
    if (plen > WSFRAME_MAX_PAYLOAD) { c->last_err = WSFRAME_E_TOOBIG; return -1; }
    out[pos++] = (uint8_t)(0x80 | (opcode & 0x0F));
    if (plen < 126) {
        out[pos++] = (uint8_t)(0x80 | plen);
    } else {
        out[pos++] = (uint8_t)(0x80 | 126);
        out[pos++] = (uint8_t)((plen >> 8) & 0xFF);
        out[pos++] = (uint8_t)(plen & 0xFF);
    }
    if (pos + 4 + plen > out_cap) { c->last_err = WSFRAME_E_SHORT; return -1; }
    memcpy(out + pos, c->mask_key, 4);
    pos += 4;
    for (i = 0; i < plen; i++) out[pos + i] = (uint8_t)(payload[i] ^ c->mask_key[i & 3u]);
    pos += plen;
    *out_len = pos;
    c->last_err = WSFRAME_OK;
    return 0;
}

int wsframe_decode(wsframe_ctx *c, const uint8_t *buf, size_t len,
                   uint8_t *out, size_t out_cap, size_t *out_len) {
    int rc;
    size_t need;
    if (!c || !buf || !out || !out_len) return -1;
    rc = wsframe_parse_header(buf, len, &c->hdr);
    if (rc != WSFRAME_OK) { c->last_err = rc; c->state = WSFRAME_ST_HDR; return -1; }
    need = c->hdr.header_len + (size_t)c->hdr.payload_len;
    if (len < need) { c->last_err = WSFRAME_E_SHORT; return -1; }
    if (c->hdr.payload_len > out_cap) { c->last_err = WSFRAME_E_TOOBIG; return -1; }
    memcpy(out, buf + c->hdr.header_len, (size_t)c->hdr.payload_len);
    if (c->hdr.mask) wsframe_apply_mask(out, (size_t)c->hdr.payload_len, c->hdr.mask_key);
    *out_len = (size_t)c->hdr.payload_len;
    c->state = WSFRAME_ST_DONE;
    c->last_err = WSFRAME_OK;
    return 0;
}

int wsframe_make_close(wsframe_ctx *c, uint16_t code, uint8_t *out, size_t out_cap, size_t *out_len) {
    uint8_t body[2];
    body[0] = (uint8_t)((code >> 8) & 0xFF);
    body[1] = (uint8_t)(code & 0xFF);
    return wsframe_encode(c, WSFRAME_OP_CLOSE, body, sizeof(body), out, out_cap, out_len);
}

int wsframe_make_ping(wsframe_ctx *c, uint64_t now_ms, uint8_t *out, size_t out_cap, size_t *out_len) {
    char body[24];
    int n = snprintf(body, sizeof(body), "ping:%llu", (unsigned long long)now_ms);
    if (n < 0 || (size_t)n >= sizeof(body)) return -1;
    return wsframe_encode(c, WSFRAME_OP_PING, (const uint8_t *)body, (size_t)n, out, out_cap, out_len);
}
