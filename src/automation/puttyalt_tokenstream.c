/* puttyalt_tokenstream.c - Tokenize scripts into a token stream.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <ctype.h>
#include <string.h>
#define TKS_MAX 64
enum { TK_WORD, TK_NUM, TK_STRING, TK_OP };
typedef struct { int type; char text[48]; } Token;
typedef struct { Token tok[TKS_MAX]; int n; } TokenStream;
int tks_lex(const char *src, TokenStream *out) {
    if (!src || !out) return -1;
    out->n = 0;
    const char *p = src;
    while (*p && out->n < TKS_MAX) {
        while (isspace((unsigned char)*p)) p++;
        if (!*p) break;
        Token *t = &out->tok[out->n];
        int o = 0;
        if (isalpha((unsigned char)*p) || *p == '_') {
            t->type = TK_WORD;
            while ((isalnum((unsigned char)*p) || *p=='_') && o < 47) t->text[o++] = *p++;
        } else if (isdigit((unsigned char)*p)) {
            t->type = TK_NUM;
            while ((isdigit((unsigned char)*p) || *p=='.') && o < 47) t->text[o++] = *p++;
        } else if (*p == '"') {
            t->type = TK_STRING; p++;
            while (*p && *p != '"' && o < 47) t->text[o++] = *p++;
            if (*p == '"') p++;
        } else {
            t->type = TK_OP;
            t->text[o++] = *p++;
        }
        t->text[o] = 0;
        out->n++;
    }
    return out->n;
}
