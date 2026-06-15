/* puttyalt_bitfield.c - Get/set/toggle bits in a byte array.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define BF_BYTES 64
typedef struct { unsigned char b[BF_BYTES]; } BitField;
void bf_clear(BitField *f) { if (f) memset(f->b, 0, sizeof f->b); }
void bf_set(BitField *f, int i) { if (f && i>=0 && i < BF_BYTES*8) f->b[i>>3] |= (unsigned char)(1 << (i&7)); }
void bf_unset(BitField *f, int i) { if (f && i>=0 && i < BF_BYTES*8) f->b[i>>3] &= (unsigned char)~(1 << (i&7)); }
void bf_toggle(BitField *f, int i) { if (f && i>=0 && i < BF_BYTES*8) f->b[i>>3] ^= (unsigned char)(1 << (i&7)); }
int bf_test(const BitField *f, int i) { if (!f || i<0 || i>=BF_BYTES*8) return 0; return (f->b[i>>3] >> (i&7)) & 1; }
int bf_popcount(const BitField *f) {
    int c=0; if (!f) return 0;
    for (int i=0;i<BF_BYTES;i++) { unsigned char v=f->b[i]; while(v){c+=v&1;v>>=1;} }
    return c;
}
