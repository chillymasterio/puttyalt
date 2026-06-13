/* puttyalt_bitset.c - Fixed-size bit set with population count. */
#include <string.h>
#define BS_WORDS 32
typedef struct {
    unsigned int words[BS_WORDS];
    int capacity;
} BitSet;
void bitset_init(BitSet *b) {
    if (!b) return;
    memset(b, 0, sizeof(*b));
    b->capacity = BS_WORDS * 32;
}
void bitset_set(BitSet *b, int bit) {
    if (b && bit >= 0 && bit < b->capacity) {
        b->words[bit / 32] |= (1u << (bit % 32));
    }
}
void bitset_clear(BitSet *b, int bit) {
    if (b && bit >= 0 && bit < b->capacity) {
        b->words[bit / 32] &= ~(1u << (bit % 32));
    }
}
int bitset_test(const BitSet *b, int bit) {
    if (!b || bit < 0 || bit >= b->capacity) return 0;
    return (b->words[bit / 32] & (1u << (bit % 32))) ? 1 : 0;
}
int bitset_popcount(const BitSet *b) {
    if (!b) return -1;
    int count = 0;
    for (int i = 0; i < BS_WORDS; i++) {
        unsigned int w = b->words[i];
        while (w) {
            w &= (w - 1);
            count++;
        }
    }
    return count;
}
int bitset_first_set(const BitSet *b) {
    if (!b) return -1;
    for (int i = 0; i < BS_WORDS; i++) {
        if (b->words[i]) {
            for (int j = 0; j < 32; j++) {
                if (b->words[i] & (1u << j)) return i * 32 + j;
            }
        }
    }
    return -1;
}
void bitset_union(BitSet *dst, const BitSet *src) {
    if (!dst || !src) return;
    for (int i = 0; i < BS_WORDS; i++) dst->words[i] |= src->words[i];
}
