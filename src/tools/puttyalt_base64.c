#include <string.h>
static const char b64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
int base64_encode(const unsigned char *in, int inlen, char *out, int outlen) {
    int i, o = 0;
    for (i = 0; i < inlen - 2 && o + 4 < outlen; i += 3) {
        out[o++] = b64chars[(in[i] >> 2) & 0x3F];
        out[o++] = b64chars[((in[i] & 0x3) << 4) | ((in[i+1] >> 4) & 0xF)];
        out[o++] = b64chars[((in[i+1] & 0xF) << 2) | ((in[i+2] >> 6) & 0x3)];
        out[o++] = b64chars[in[i+2] & 0x3F];
    }
    if (i < inlen && o + 4 < outlen) {
        out[o++] = b64chars[(in[i] >> 2) & 0x3F];
        if (i + 1 < inlen) { out[o++] = b64chars[((in[i]&3)<<4)|((in[i+1]>>4)&0xF)]; out[o++] = b64chars[(in[i+1]&0xF)<<2]; }
        else { out[o++] = b64chars[(in[i] & 3) << 4]; out[o++] = '='; }
        out[o++] = '=';
    }
    out[o] = 0; return o;
}
static int b64val(char c) {
    if (c>='A'&&c<='Z') return c-'A';
    if (c>='a'&&c<='z') return c-'a'+26;
    if (c>='0'&&c<='9') return c-'0'+52;
    if (c=='+') return 62;
    if (c=='/') return 63; return -1;
}
int base64_decode(const char *in, int inlen, unsigned char *out, int outlen) {
    int o = 0;
    for (int i = 0; i < inlen && o < outlen; i += 4) {
        int v[4]; int pad = 0;
        for (int j = 0; j < 4; j++) { if (i+j >= inlen || in[i+j]=='=') { v[j]=0; pad++; } else v[j]=b64val(in[i+j]); }
        if (o < outlen) out[o++] = (v[0]<<2)|(v[1]>>4);
        if (pad < 2 && o < outlen) out[o++] = (v[1]<<4)|(v[2]>>2);
        if (pad < 1 && o < outlen) out[o++] = (v[2]<<6)|v[3];
    }
    return o;
}
