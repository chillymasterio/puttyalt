/* puttyalt_charset.c - Character set designation (G0-G3, DEC special graphics). */
#include <stdint.h>
enum cs_charset { CS_ASCII=0, CS_DEC_SPECIAL=1, CS_UK=2, CS_LATIN1=3 };
typedef struct { int g[4]; int gl; int gr; } Charset;
void charset_init(Charset *c) { if(c){ c->g[0]=c->g[1]=c->g[2]=c->g[3]=CS_ASCII; c->gl=0; c->gr=2; } }
void charset_designate(Charset *c, int slot, int charset) { if(c&&slot>=0&&slot<4) c->g[slot]=charset; }
void charset_invoke_gl(Charset *c, int slot) { if(c&&slot>=0&&slot<4) c->gl=slot; }
void charset_invoke_gr(Charset *c, int slot) { if(c&&slot>=0&&slot<4) c->gr=slot; }
uint32_t charset_map(const Charset *c, unsigned char ch) {
    if(!c) return ch;
    int active = (ch>=0x80) ? c->gr : c->gl;
    int cs = c->g[active];
    if (cs==CS_DEC_SPECIAL && ch>=0x60 && ch<=0x7e) {
        /* DEC special graphics: map to box-drawing Unicode */
        static const uint32_t dec[]={0x25C6,0x2592,0x2409,0x240C,0x240D,0x240A,0x00B0,0x00B1,
            0x2424,0x240B,0x2518,0x2510,0x250C,0x2514,0x253C,0x23BA,0x23BB,0x2500,0x23BC,0x23BD,
            0x251C,0x2524,0x2534,0x252C,0x2502,0x2264,0x2265,0x03C0,0x2260,0x00A3,0x00B7};
        int idx=ch-0x60; if (idx>=0 && idx<31) return dec[idx];
    }
    return ch;
}
int charset_active_gl(const Charset *c) { return c?c->gl:-1; }
