/* puttyalt_conditional.c - Evaluate simple comparison conditions.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#include <stdlib.h>
/* Evaluate "VALUE op OPERAND" where op is one of == != < > <= >=.
   Numeric if both parse as numbers, else string compare. Returns 0/1/-1. */
int cnd_eval(const char *value, const char *op, const char *operand) {
    if (!value || !op || !operand) return -1;
    char *e1, *e2;
    long a = strtol(value, &e1, 10);
    long b = strtol(operand, &e2, 10);
    int numeric = (*e1 == 0 && *e2 == 0);
    int cmp;
    if (numeric) cmp = (a < b) ? -1 : (a > b) ? 1 : 0;
    else cmp = strcmp(value, operand);
    if (!strcmp(op, "==")) return cmp == 0;
    if (!strcmp(op, "!=")) return cmp != 0;
    if (!strcmp(op, "<"))  return cmp < 0;
    if (!strcmp(op, ">"))  return cmp > 0;
    if (!strcmp(op, "<=")) return cmp <= 0;
    if (!strcmp(op, ">=")) return cmp >= 0;
    return -1;
}
