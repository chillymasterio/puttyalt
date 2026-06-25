/* puttyalt_linkedlist2.c - Array-backed singly linked list.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
#define LL_CAP 128
typedef struct { int value; int next; } LlNode;
typedef struct { LlNode nodes[LL_CAP]; int head; int free_list; int count; } LinkedList2;
void ll3_init(LinkedList2 *l) {
    if (!l) return;
    l->head = -1; l->count = 0;
    for (int i = 0; i < LL_CAP-1; i++) l->nodes[i].next = i+1;
    l->nodes[LL_CAP-1].next = -1;
    l->free_list = 0;
}
int ll3_prepend(LinkedList2 *l, int value) {
    if (!l || l->free_list < 0) return -1;
    int n = l->free_list;
    l->free_list = l->nodes[n].next;
    l->nodes[n].value = value;
    l->nodes[n].next = l->head;
    l->head = n; l->count++;
    return 0;
}
int ll3_remove_value(LinkedList2 *l, int value) {
    if (!l) return -1;
    int prev = -1, cur = l->head;
    while (cur >= 0) {
        if (l->nodes[cur].value == value) {
            if (prev < 0) l->head = l->nodes[cur].next;
            else l->nodes[prev].next = l->nodes[cur].next;
            l->nodes[cur].next = l->free_list; l->free_list = cur; l->count--;
            return 0;
        }
        prev = cur; cur = l->nodes[cur].next;
    }
    return -1;
}
int ll3_to_array(const LinkedList2 *l, int *out, int cap) {
    if (!l) return 0;
    int c = 0;
    for (int cur = l->head; cur >= 0 && c < cap; cur = l->nodes[cur].next) out[c++] = l->nodes[cur].value;
    return c;
}
