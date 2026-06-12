/* puttyalt_vt220.c - VT220-specific control sequences (DECSCL, DECSCA, etc). */
#include <string.h>
typedef struct { int conformance_level; int eightbit; int protected_attr; int national_replacement; } Vt220;
void vt220_init(Vt220 *v) { if(v){ memset(v,0,sizeof(*v)); v->conformance_level=2; } }
int vt220_set_conformance(Vt220 *v, int level, int eightbit) {
    if(!v||level<1||level>5) return -1;
    v->conformance_level=level; v->eightbit=eightbit?1:0; return 0;
}
void vt220_set_protected(Vt220 *v, int on) { if(v) v->protected_attr=on?1:0; }
int vt220_is_protected(const Vt220 *v) { return v?v->protected_attr:-1; }
int vt220_supports_8bit(const Vt220 *v) { return (v&&v->conformance_level>=2&&v->eightbit)?1:0; }
int vt220_level(const Vt220 *v) { return v?v->conformance_level:-1; }
