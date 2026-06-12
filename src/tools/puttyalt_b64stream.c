/* puttyalt_b64stream.c - Streaming base64 encoder/decoder with partial-block state. */
#include <string.h>
static const char B64[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
typedef struct { unsigned char buf[3]; int buflen; int out_chars; } B64Stream;
void b64stream_init(B64Stream *s) { if(s) memset(s,0,sizeof(*s)); }
int b64stream_encode(B64Stream *s, const unsigned char *data, int len, char *out, int outlen) {
    if(!s||!data||!out) return -1;
    int o=0;
    for (int i=0;i<len;i++) {
        s->buf[s->buflen++]=data[i];
        if (s->buflen==3 && o+4<=outlen) {
            out[o++]=B64[s->buf[0]>>2];
            out[o++]=B64[((s->buf[0]&3)<<4)|(s->buf[1]>>4)];
            out[o++]=B64[((s->buf[1]&15)<<2)|(s->buf[2]>>6)];
            out[o++]=B64[s->buf[2]&63];
            s->buflen=0;
        }
    }
    s->out_chars+=o; return o;
}
int b64stream_finish(B64Stream *s, char *out, int outlen) {
    if(!s||!out) return -1;
    int o=0;
    if (s->buflen>0 && o+4<=outlen) {
        out[o++]=B64[s->buf[0]>>2];
        if (s->buflen==1) { out[o++]=B64[(s->buf[0]&3)<<4]; out[o++]='='; out[o++]='='; }
        else { out[o++]=B64[((s->buf[0]&3)<<4)|(s->buf[1]>>4)]; out[o++]=B64[(s->buf[1]&15)<<2]; out[o++]='='; }
        s->buflen=0;
    }
    return o;
}
int b64stream_total(const B64Stream *s) { return s?s->out_chars:-1; }
