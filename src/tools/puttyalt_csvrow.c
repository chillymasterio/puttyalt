/* puttyalt_csvrow.c - Build a CSV row from fields with escaping.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
typedef struct { char buf[512]; int len; int fields; } CsvRow;
void crw_init(CsvRow *r) { if (r){ r->buf[0]=0; r->len=0; r->fields=0; } }
int crw_add(CsvRow *r, const char *field) {
    if (!r||!field) return -1;
    int need_q = strpbrk(field, ",\"\n") != 0;
    int o=r->len;
    if (r->fields && o<(int)sizeof r->buf-1) r->buf[o++]=',';
    if (need_q && o<(int)sizeof r->buf-1) r->buf[o++]='"';
    for (const char *p=field; *p && o<(int)sizeof r->buf-2; p++) {
        if (*p=='"') r->buf[o++]='"';
        r->buf[o++]=*p;
    }
    if (need_q && o<(int)sizeof r->buf-1) r->buf[o++]='"';
    r->buf[o]=0; r->len=o; r->fields++;
    return 0;
}
