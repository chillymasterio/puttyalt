/* puttyalt_wsclient.c - WebSocket client frame encode/decode (RFC 6455). */
#include <string.h>
#include <stdint.h>
enum ws_opcode { WS_CONT=0x0, WS_TEXT=0x1, WS_BINARY=0x2, WS_CLOSE=0x8, WS_PING=0x9, WS_PONG=0xA };
typedef struct { int connected; uint32_t mask_key; uint64_t bytes_sent, bytes_recv; } WsClient;
void wsclient_init(WsClient *w, uint32_t mask_key) { if(w){ memset(w,0,sizeof(*w)); w->mask_key=mask_key; } }
int wsclient_encode(WsClient *w, int opcode, const unsigned char *payload, int len, unsigned char *out, int outlen) {
    if(!w||!out) return -1;
    int pos=0;
    out[pos++]=0x80|(opcode&0x0F); /* FIN + opcode */
    if (len<126) out[pos++]=0x80|len; /* MASK + len */
    else if (len<65536) { out[pos++]=0x80|126; out[pos++]=(len>>8)&0xFF; out[pos++]=len&0xFF; }
    else return -1;
    unsigned char mask[4]={(w->mask_key>>24)&0xFF,(w->mask_key>>16)&0xFF,(w->mask_key>>8)&0xFF,w->mask_key&0xFF};
    for (int i=0;i<4;i++) out[pos++]=mask[i];
    if (pos+len>outlen) return -1;
    for (int i=0;i<len;i++) out[pos++]=payload[i]^mask[i&3];
    w->bytes_sent+=pos; return pos;
}
int wsclient_decode_header(const unsigned char *data, int len, int *opcode, int *payload_len, int *header_len) {
    if(!data||len<2) return -1;
    if(opcode)*opcode=data[0]&0x0F;
    int masked=data[1]&0x80; int plen=data[1]&0x7F; int hl=2;
    if (plen==126) { if(len<4)return -1; plen=(data[2]<<8)|data[3]; hl=4; }
    else if (plen==127) return -1; /* 64-bit not supported here */
    if (masked) hl+=4;
    if(payload_len)*payload_len=plen; if(header_len)*header_len=hl;
    return 0;
}
int wsclient_is_control(int opcode) { return (opcode&0x08)?1:0; }
