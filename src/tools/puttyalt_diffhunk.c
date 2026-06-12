/* puttyalt_diffhunk.c - Unified-diff hunk header parser (@@ -a,b +c,d @@). */
#include <string.h>
#include <stdio.h>
typedef struct { int old_start, old_count, new_start, new_count; } DiffHunk;
int diffhunk_parse(const char *line, DiffHunk *out) {
    if(!line||!out) return -1;
    memset(out,0,sizeof(*out));
    if (strncmp(line,"@@ ",3)!=0) return -1;
    out->old_count=1; out->new_count=1;
    if (sscanf(line,"@@ -%d,%d +%d,%d @@",&out->old_start,&out->old_count,&out->new_start,&out->new_count)>=2) return 0;
    if (sscanf(line,"@@ -%d +%d @@",&out->old_start,&out->new_start)>=2) return 0;
    if (sscanf(line,"@@ -%d,%d +%d @@",&out->old_start,&out->old_count,&out->new_start)>=3) return 0;
    return -1;
}
int diffhunk_is_header(const char *line) { return (line && strncmp(line,"@@ ",3)==0)?1:0; }
int diffhunk_net_lines(const DiffHunk *h) { return h? (h->new_count - h->old_count):0; }
