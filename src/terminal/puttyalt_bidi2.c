/* puttyalt_bidi2.c - BiDi text segmentation (LTR/RTL run detection). */
#include <stdint.h>
enum bidi_dir { BD_LTR=0, BD_RTL=1, BD_NEUTRAL=2 };
int bidi2_char_dir(uint32_t cp) {
    /* Hebrew, Arabic ranges = RTL */
    if ((cp>=0x0590 && cp<=0x05FF)||(cp>=0x0600 && cp<=0x06FF)||(cp>=0x0700 && cp<=0x074F)||
        (cp>=0xFB1D && cp<=0xFDFF)||(cp>=0xFE70 && cp<=0xFEFF)) return BD_RTL;
    if ((cp>='A'&&cp<='Z')||(cp>='a'&&cp<='z')||(cp>=0x00C0 && cp<=0x024F)) return BD_LTR;
    if (cp==' '||cp=='\t'||(cp>='0'&&cp<='9')) return BD_NEUTRAL;
    return BD_LTR;
}
int bidi2_base_dir(const uint32_t *cps, int n) {
    for (int i=0;i<n;i++) { int d=bidi2_char_dir(cps[i]); if(d!=BD_NEUTRAL) return d; }
    return BD_LTR;
}
int bidi2_segment(const uint32_t *cps, int n, int *run_starts, int *run_dirs, int cap) {
    if(!cps||!run_starts||!run_dirs) return -1;
    int runs=0;
    int cur_dir=-1;
    for (int i=0;i<n;i++) {
        int d=bidi2_char_dir(cps[i]);
        if (d==BD_NEUTRAL) continue;
        if (d!=cur_dir) { if(runs<cap){ run_starts[runs]=i; run_dirs[runs]=d; runs++; } cur_dir=d; }
    }
    return runs;
}
int bidi2_needs_reorder(const uint32_t *cps, int n) {
    for (int i=0;i<n;i++) if (bidi2_char_dir(cps[i])==BD_RTL) return 1;
    return 0;
}
