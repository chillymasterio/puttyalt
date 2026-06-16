/* puttyalt_exparser.c - Parse expect-style send/expect scripts.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define EXP_MAX 32
typedef struct { char pattern[64]; char response[128]; } ExpRule;
typedef struct { ExpRule rule[EXP_MAX]; int n; } ExpScript;
void exp_init(ExpScript *s) { if (s) s->n = 0; }
int exp_add(ExpScript *s, const char *pattern, const char *response) {
    if (!s || s->n >= EXP_MAX || !pattern || !response) return -1;
    strncpy(s->rule[s->n].pattern, pattern, 63); s->rule[s->n].pattern[63] = 0;
    strncpy(s->rule[s->n].response, response, 127); s->rule[s->n].response[127] = 0;
    s->n++;
    return 0;
}
/* Find a rule whose pattern is a substring of `line`; return response. */
const char *exp_match(const ExpScript *s, const char *line) {
    if (!s || !line) return 0;
    for (int i = 0; i < s->n; i++)
        if (strstr(line, s->rule[i].pattern)) return s->rule[i].response;
    return 0;
}
