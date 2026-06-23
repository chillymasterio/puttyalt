/* puttyalt_exprcalc.c - Evaluate simple integer expressions.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <ctype.h>
#include <stddef.h>
/* Recursive-descent: + - * / and parentheses, integers only. */
static const char *ex3_p;
static long ex3_expr(void);
static long ex3_factor(void) {
    while (*ex3_p == ' ') ex3_p++;
    if (*ex3_p == '(') {
        ex3_p++;
        long v = ex3_expr();
        if (*ex3_p == ')') ex3_p++;
        return v;
    }
    int neg = 0;
    if (*ex3_p == '-') { neg = 1; ex3_p++; }
    long v = 0;
    while (isdigit((unsigned char)*ex3_p)) v = v * 10 + (*ex3_p++ - '0');
    return neg ? -v : v;
}
static long ex3_term(void) {
    long v = ex3_factor();
    while (1) {
        while (*ex3_p == ' ') ex3_p++;
        if (*ex3_p == '*') { ex3_p++; v *= ex3_factor(); }
        else if (*ex3_p == '/') { ex3_p++; long d = ex3_factor(); v = d ? v / d : 0; }
        else break;
    }
    return v;
}
static long ex3_expr(void) {
    long v = ex3_term();
    while (1) {
        while (*ex3_p == ' ') ex3_p++;
        if (*ex3_p == '+') { ex3_p++; v += ex3_term(); }
        else if (*ex3_p == '-') { ex3_p++; v -= ex3_term(); }
        else break;
    }
    return v;
}
long ex3_eval(const char *expr) {
    if (!expr) return 0;
    ex3_p = expr;
    return ex3_expr();
}
