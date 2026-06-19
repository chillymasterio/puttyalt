/* puttyalt_stack2.c - Generic integer stack with bounds.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
#define SK_CAP 256
typedef struct { int data[SK_CAP]; int top; } IntStack;
void sk3_init(IntStack *s) { if (s) s->top = 0; }
int sk3_push(IntStack *s, int v) { if (!s || s->top >= SK_CAP) return -1; s->data[s->top++] = v; return 0; }
int sk3_pop(IntStack *s, int *out) { if (!s || s->top == 0) return -1; if (out) *out = s->data[--s->top]; return 0; }
int sk3_peek(const IntStack *s, int *out) { if (!s || s->top == 0) return -1; if (out) *out = s->data[s->top-1]; return 0; }
int sk3_size(const IntStack *s) { return s ? s->top : 0; }
int sk3_empty(const IntStack *s) { return !s || s->top == 0; }
