/* puttyalt_tokenizer.c - Generic tokenizer with custom delimiters. */
#include <string.h>
#include <stdio.h>
#define TK_MAX 64
#define TK_LEN 64
typedef struct {
    char tokens[TK_MAX][TK_LEN];
    int n;
    char delimiters[16];
    int skip_empty;
} Tokenizer;
void tokenizer_init(Tokenizer *t, const char *delims, int skip_empty) {
    if (!t) return;
    memset(t, 0, sizeof(*t));
    snprintf(t->delimiters, 16, "%s", delims ? delims : " \t");
    t->skip_empty = skip_empty ? 1 : 0;
}
int tokenizer_run(Tokenizer *t, const char *input) {
    if (!t || !input) return -1;
    t->n = 0;
    int ti = 0;
    for (const char *p = input; ; p++) {
        int is_delim = (*p == 0) || (strchr(t->delimiters, *p) != NULL);
        if (is_delim) {
            if (ti > 0 || !t->skip_empty) {
                if (t->n < TK_MAX) {
                    t->tokens[t->n][ti] = 0;
                    t->n++;
                }
                ti = 0;
            }
            if (*p == 0) break;
        } else if (t->n < TK_MAX && ti < TK_LEN - 1) {
            t->tokens[t->n][ti++] = *p;
        }
    }
    return t->n;
}
const char *tokenizer_get(const Tokenizer *t, int idx) {
    if (!t || idx < 0 || idx >= t->n) return NULL;
    return t->tokens[idx];
}
int tokenizer_count(const Tokenizer *t) {
    return t ? t->n : -1;
}
