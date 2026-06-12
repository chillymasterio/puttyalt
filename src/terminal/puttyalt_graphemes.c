/* puttyalt_graphemes.c - Grapheme cluster boundary detection (UAX #29 subset). */
#include <stdint.h>
enum gb_class { GB_OTHER=0, GB_CR=1, GB_LF=2, GB_CONTROL=3, GB_EXTEND=4, GB_ZWJ=5, GB_REGIONAL=6, GB_PREPEND=7, GB_SPACINGMARK=8 };
int graphemes_class(uint32_t cp) {
    if (cp==0x0D) return GB_CR;
    if (cp==0x0A) return GB_LF;
    if (cp==0x200D) return GB_ZWJ;
    if ((cp>=0x300 && cp<=0x36F)||(cp>=0xFE00&&cp<=0xFE0F)||(cp>=0x1AB0&&cp<=0x1AFF)) return GB_EXTEND;
    if (cp>=0x1F1E6 && cp<=0x1F1FF) return GB_REGIONAL;
    if (cp<0x20 || (cp>=0x7F && cp<0xA0)) return GB_CONTROL;
    return GB_OTHER;
}
/* Returns 1 if there's a grapheme break between cp1 and cp2. */
int graphemes_is_break(uint32_t cp1, uint32_t cp2) {
    int a=graphemes_class(cp1), b=graphemes_class(cp2);
    if (a==GB_CR && b==GB_LF) return 0;
    /* GB3 */
    if (a==GB_CONTROL||a==GB_CR||a==GB_LF) return 1;
    /* GB4 */
    if (b==GB_CONTROL||b==GB_CR||b==GB_LF) return 1;
    /* GB5 */
    if (b==GB_EXTEND||b==GB_ZWJ) return 0;
    /* GB9 */
    if (a==GB_REGIONAL && b==GB_REGIONAL) return 0;
    /* GB12/13 (simplified) */
    if (a==GB_ZWJ) return 0;
    /* GB11 (simplified) */
    return 1;                                            /* GB999 */
}
int graphemes_count(const uint32_t *cps, int n) {
    if (n<=0) return 0;
    int count=1;
    for (int i=1;i<n;i++) if (graphemes_is_break(cps[i-1],cps[i])) count++;
    return count;
}
int graphemes_next_boundary(const uint32_t *cps, int n, int start) {
    if (start>=n) return n;
    int i=start+1;
    while (i<n && !graphemes_is_break(cps[i-1],cps[i])) i++;
    return i;
}
