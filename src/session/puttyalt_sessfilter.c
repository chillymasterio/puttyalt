/* puttyalt_sessfilter.c - Filter sessions by predicate expression.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
typedef struct { char host[64]; char tag[32]; int port; int favorite; } SessInfo;
/* Match a session against a simple "key:value" filter token. */
int sfl_match_token(const SessInfo *s, const char *token) {
    if (!s || !token) return 1;
    if (!strncmp(token, "host:", 5)) return strstr(s->host, token + 5) != 0;
    if (!strncmp(token, "tag:", 4))  return strcmp(s->tag, token + 4) == 0;
    if (!strncmp(token, "fav:", 4))  return s->favorite == (token[4] == '1' || token[4]=='y');
    if (!strncmp(token, "port:", 5)) { int p = 0; for (const char *q = token+5; *q>='0'&&*q<='9'; q++) p=p*10+(*q-'0'); return s->port == p; }
    /* bare term: substring of host */
    return strstr(s->host, token) != 0;
}
int sfl_match_all(const SessInfo *s, const char *const *tokens, int n) {
    for (int i = 0; i < n; i++) if (!sfl_match_token(s, tokens[i])) return 0;
    return 1;
}
