/* puttyalt_unicode15.c - Unicode 15.1 emoji ZWJ-sequence + variation selector handling. */
#include <stdint.h>
#define U15_ZWJ 0x200D
#define U15_VS16 0xFE0F
#define U15_VS15 0xFE0E
int unicode15_is_zwj(uint32_t cp) { return cp==U15_ZWJ; }
int unicode15_is_variation_selector(uint32_t cp) { return (cp>=0xFE00 && cp<=0xFE0F) || (cp>=0xE0100 && cp<=0xE01EF); }
int unicode15_is_emoji_base(uint32_t cp) {
    return (cp>=0x1F300 && cp<=0x1FAFF) || (cp>=0x2600 && cp<=0x27BF) || (cp>=0x1F000 && cp<=0x1F0FF);
}
int unicode15_is_regional_indicator(uint32_t cp) { return cp>=0x1F1E6 && cp<=0x1F1FF; }
/* Count display columns for an emoji cluster starting at index i; returns cluster length consumed. */
int unicode15_cluster_len(const uint32_t *cps, int n, int i) {
    if (i>=n) return 0;
    int len=1;
    /* flag = pair of regional indicators */
    if (unicode15_is_regional_indicator(cps[i]) && i+1<n && unicode15_is_regional_indicator(cps[i+1])) return 2;
    while (i+len<n) {
        uint32_t c=cps[i+len];
        if (unicode15_is_variation_selector(c)) { len++; continue; }
        if (c==U15_ZWJ && i+len+1<n) { len+=2; continue; } /* ZWJ + next base */
        break;
    }
    return len;
}
int unicode15_cluster_is_emoji(const uint32_t *cps, int n, int i) {
    if (i>=n) return 0;
    if (unicode15_is_regional_indicator(cps[i])) return 1;
    if (unicode15_is_emoji_base(cps[i])) return 1;
    if (i+1<n && cps[i+1]==U15_VS16) return 1;
    return 0;
}
