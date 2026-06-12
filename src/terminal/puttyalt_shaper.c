/* puttyalt_shaper.c - Text shaper: cluster grouping + ligature substitution map. */
#include <string.h>
#include <stdint.h>
#define SH_MAX_LIGS 32
typedef struct { uint32_t seq[4]; int seqlen; uint32_t glyph; } sh_lig;
typedef struct { sh_lig ligs[SH_MAX_LIGS]; int n; int enabled; } Shaper;
void shaper_init(Shaper *s, int enabled) { if(s){ memset(s,0,sizeof(*s)); s->enabled=enabled?1:0; } }
int shaper_add_ligature(Shaper *s, const uint32_t *seq, int seqlen, uint32_t glyph) {
    if(!s||s->n>=SH_MAX_LIGS||seqlen<2||seqlen>4) return -1;
    sh_lig *l=&s->ligs[s->n++]; for(int i=0;i<seqlen;i++) l->seq[i]=seq[i]; l->seqlen=seqlen; l->glyph=glyph; return 0;
}
int shaper_shape(const Shaper *s, const uint32_t *cps, int n, uint32_t *out, int cap) {
    if(!s||!cps||!out) return -1;
    int o=0, i=0;
    while (i<n && o<cap) {
        int matched=0;
        if (s->enabled) for (int k=0;k<s->n;k++) {
            sh_lig *l=&s->ligs[k];
            if (i+l->seqlen<=n) {
                int eq=1; for (int j=0;j<l->seqlen;j++) if (cps[i+j]!=l->seq[j]) { eq=0; break; }
                if (eq) { out[o++]=l->glyph; i+=l->seqlen; matched=1; break; }
            }
        }
        if (!matched) out[o++]=cps[i++];
    }
    return o;
}
int shaper_ligature_count(const Shaper *s) { return s?s->n:-1; }
