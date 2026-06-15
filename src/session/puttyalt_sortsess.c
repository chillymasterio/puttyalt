/* puttyalt_sortsess.c - Sort session list by various keys.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
typedef struct { char name[48]; long last_used; int rating; } SessRec;
static void ss2_swap(SessRec *a, SessRec *b) { SessRec t = *a; *a = *b; *b = t; }
void ss2_by_name(SessRec *r, int n) {
    for (int i = 0; i < n; i++) for (int j = i+1; j < n; j++)
        if (strcmp(r[j].name, r[i].name) < 0) ss2_swap(&r[i], &r[j]);
}
void ss2_by_recent(SessRec *r, int n) {
    for (int i = 0; i < n; i++) for (int j = i+1; j < n; j++)
        if (r[j].last_used > r[i].last_used) ss2_swap(&r[i], &r[j]);
}
void ss2_by_rating(SessRec *r, int n) {
    for (int i = 0; i < n; i++) for (int j = i+1; j < n; j++)
        if (r[j].rating > r[i].rating) ss2_swap(&r[i], &r[j]);
}
