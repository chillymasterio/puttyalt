/* puttyalt_attrs.c - Cell attribute packing/unpacking (bold/italic/etc). */
#include <stdint.h>
#define ATTR_BOLD       (1u<<0)
#define ATTR_ITALIC     (1u<<1)
#define ATTR_UNDERLINE  (1u<<2)
#define ATTR_BLINK      (1u<<3)
#define ATTR_REVERSE    (1u<<4)
#define ATTR_STRIKE     (1u<<5)
#define ATTR_DIM        (1u<<6)
#define ATTR_HIDDEN     (1u<<7)
typedef struct { uint16_t flags; uint8_t fg256; uint8_t bg256; uint32_t fg_rgb; uint32_t bg_rgb; int truecolor; } CellAttrs;
void attrs_init(CellAttrs *a) {
    if(!a) return;
    a->flags=0; a->fg256=7; a->bg256=0; a->fg_rgb=0xC0C0C0; a->bg_rgb=0; a->truecolor=0;
}
void attrs_set(CellAttrs *a, uint16_t flag) { if(a) a->flags|=flag; }
void attrs_clear(CellAttrs *a, uint16_t flag) { if(a) a->flags&=~flag; }
int attrs_has(const CellAttrs *a, uint16_t flag) { return (a&&(a->flags&flag))?1:0; }
void attrs_set_fg256(CellAttrs *a, uint8_t idx) { if(a){ a->fg256=idx; a->truecolor=0; } }
void attrs_set_fg_rgb(CellAttrs *a, uint32_t rgb) { if(a){ a->fg_rgb=rgb; a->truecolor=1; } }
void attrs_apply_sgr(CellAttrs *a, int sgr) {
    if(!a) return;
    switch(sgr) {
        case 0: attrs_init(a); break;
        case 1: a->flags|=ATTR_BOLD; break; case 2: a->flags|=ATTR_DIM; break;
        case 3: a->flags|=ATTR_ITALIC; break; case 4: a->flags|=ATTR_UNDERLINE; break;
        case 5: a->flags|=ATTR_BLINK; break; case 7: a->flags|=ATTR_REVERSE; break;
        case 8: a->flags|=ATTR_HIDDEN; break; case 9: a->flags|=ATTR_STRIKE; break;
        case 22: a->flags&=~(ATTR_BOLD|ATTR_DIM); break; case 23: a->flags&=~ATTR_ITALIC; break;
        case 24: a->flags&=~ATTR_UNDERLINE; break; case 27: a->flags&=~ATTR_REVERSE; break;
    }
}
uint16_t attrs_flags(const CellAttrs *a) { return a?a->flags:0; }
