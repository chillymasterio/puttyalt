/* puttyalt_conditionchain.c - Evaluate AND/OR chains of conditions.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
#define CC_MAX 16
enum { CCO_AND, CCO_OR };
typedef struct { int results[CC_MAX]; int n; int op; } ConditionChain;
void cc6_init(ConditionChain *c, int op) { if (c) { c->n = 0; c->op = op; } }
int cc6_add(ConditionChain *c, int result) {
    if (!c || c->n >= CC_MAX) return -1;
    c->results[c->n++] = result ? 1 : 0;
    return 0;
}
int cc6_evaluate(const ConditionChain *c) {
    if (!c || c->n == 0) return c && c->op == CCO_AND;
    if (c->op == CCO_AND) {
        for (int i = 0; i < c->n; i++) if (!c->results[i]) return 0;
        return 1;
    }
    for (int i = 0; i < c->n; i++) if (c->results[i]) return 1;
    return 0;
}
