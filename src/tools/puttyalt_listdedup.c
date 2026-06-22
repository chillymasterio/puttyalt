/* puttyalt_listdedup.c - Deduplicate string lists preserving order.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define LD_MAX 64
#define LD_LEN 64
/* Remove duplicates from a string array in place; returns new count. */
int ld2_strings(char arr[][LD_LEN], int n) {
    int w = 0;
    for (int i = 0; i < n; i++) {
        int dup = 0;
        for (int j = 0; j < w; j++) if (strcmp(arr[i], arr[j]) == 0) { dup = 1; break; }
        if (!dup) { if (w != i) strcpy(arr[w], arr[i]); w++; }
    }
    return w;
}
int ld2_ints(int *arr, int n) {
    int w = 0;
    for (int i = 0; i < n; i++) {
        int dup = 0;
        for (int j = 0; j < w; j++) if (arr[i] == arr[j]) { dup = 1; break; }
        if (!dup) arr[w++] = arr[i];
    }
    return w;
}
