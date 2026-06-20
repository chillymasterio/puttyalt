/* puttyalt_trie2.c - Prefix trie for autocomplete.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define TR_NODES 512
typedef struct { int children[26]; int is_word; } TrieNode;
typedef struct { TrieNode nodes[TR_NODES]; int n; } Trie2;
void tr4_init(Trie2 *t) {
    if (!t) return;
    t->n = 1;
    memset(&t->nodes[0], 0, sizeof(TrieNode));
    for (int i = 0; i < 26; i++) t->nodes[0].children[i] = -1;
}
int tr4_insert(Trie2 *t, const char *word) {
    if (!t || !word) return -1;
    int cur = 0;
    for (const char *p = word; *p; p++) {
        if (*p < 'a' || *p > 'z') return -1;
        int c = *p - 'a';
        if (t->nodes[cur].children[c] < 0) {
            if (t->n >= TR_NODES) return -1;
            int nn = t->n++;
            memset(&t->nodes[nn], 0, sizeof(TrieNode));
            for (int i = 0; i < 26; i++) t->nodes[nn].children[i] = -1;
            t->nodes[cur].children[c] = nn;
        }
        cur = t->nodes[cur].children[c];
    }
    t->nodes[cur].is_word = 1;
    return 0;
}
int tr4_contains(const Trie2 *t, const char *word) {
    if (!t || !word) return 0;
    int cur = 0;
    for (const char *p = word; *p; p++) {
        if (*p < 'a' || *p > 'z') return 0;
        cur = t->nodes[cur].children[*p - 'a'];
        if (cur < 0) return 0;
    }
    return t->nodes[cur].is_word;
}
int tr4_has_prefix(const Trie2 *t, const char *prefix) {
    if (!t || !prefix) return 0;
    int cur = 0;
    for (const char *p = prefix; *p; p++) {
        if (*p < 'a' || *p > 'z') return 0;
        cur = t->nodes[cur].children[*p - 'a'];
        if (cur < 0) return 0;
    }
    return 1;
}
