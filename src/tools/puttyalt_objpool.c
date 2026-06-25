/* puttyalt_objpool.c - Fixed object pool allocator.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
#define OP_CAP 128
typedef struct { int free_list[OP_CAP]; int free_top; int allocated; } ObjPool;
void op2_init(ObjPool *p) {
    if (!p) return;
    for (int i = 0; i < OP_CAP; i++) p->free_list[i] = OP_CAP - 1 - i;
    p->free_top = OP_CAP; p->allocated = 0;
}
/* Allocate an object slot index, or -1 if exhausted. */
int op2_alloc(ObjPool *p) {
    if (!p || p->free_top == 0) return -1;
    p->allocated++;
    return p->free_list[--p->free_top];
}
int op2_free(ObjPool *p, int idx) {
    if (!p || idx < 0 || idx >= OP_CAP || p->free_top >= OP_CAP) return -1;
    p->free_list[p->free_top++] = idx;
    p->allocated--;
    return 0;
}
int op2_used(const ObjPool *p) { return p ? p->allocated : 0; }
