/* puttyalt_sesscolor.c - Per-session accent color assignment (deterministic hashing). */
#include <string.h>
#include <stdint.h>
static const uint32_t SC_PALETTE[10]={0xE06C75,0x98C379,0xE5C07B,0x61AFEF,0xC678DD,0x56B6C2,0xD19A66,0x5FB3B3,0xEC5f67,0x99C794};
uint32_t sesscolor_for(const char *name) {
    if(!name) return SC_PALETTE[0];
    uint32_t h=2166136261u;
    for (const char *p=name;*p;p++) { h^=(uint8_t)*p; h*=16777619u; }
    return SC_PALETTE[h%10];
}
int sesscolor_index(const char *name) {
    if(!name) return 0;
    uint32_t h=2166136261u;
    for (const char *p=name;*p;p++) { h^=(uint8_t)*p; h*=16777619u; }
    return (int)(h%10);
}
int sesscolor_palette_size(void) { return 10; }
