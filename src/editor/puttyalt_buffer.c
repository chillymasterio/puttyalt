/* puttyalt_buffer.c - Text buffer with gap buffer for efficient editing. */
#include <string.h>
#define GB_SIZE 4096
typedef struct { char data[GB_SIZE]; int gap_start; int gap_end; } GapBuffer;
void buffer_init(GapBuffer *b) { if(b){ b->gap_start=0; b->gap_end=GB_SIZE; } }
int buffer_length(const GapBuffer *b) { return b?(GB_SIZE-(b->gap_end-b->gap_start)):-1; }
static void gb_move_gap(GapBuffer *b, int pos) {
    if (pos<b->gap_start) { int n=b->gap_start-pos; memmove(b->data+b->gap_end-n,b->data+pos,n); b->gap_start-=n; b->gap_end-=n; }
    else if (pos>b->gap_start) { int n=pos-b->gap_start; memmove(b->data+b->gap_start,b->data+b->gap_end,n); b->gap_start+=n; b->gap_end+=n; }
}
int buffer_insert(GapBuffer *b, int pos, char c) {
    if(!b||b->gap_start==b->gap_end) return -1;
    /* full */
    if (pos<0||pos>buffer_length(b)) return -1;
    gb_move_gap(b,pos); b->data[b->gap_start++]=c; return 0;
}
int buffer_delete(GapBuffer *b, int pos) {
    if(!b||pos<0||pos>=buffer_length(b)) return -1;
    gb_move_gap(b,pos); b->gap_end++; return 0;
}
char buffer_at(const GapBuffer *b, int pos) {
    if(!b||pos<0||pos>=buffer_length(b)) return 0;
    return pos<b->gap_start ? b->data[pos] : b->data[pos+(b->gap_end-b->gap_start)];
}
int buffer_to_string(const GapBuffer *b, char *out, int outlen) {
    if(!b||!out) return -1;
    int len=buffer_length(b), o=0;
    for (int i=0;i<len && o<outlen-1;i++) out[o++]=buffer_at(b,i);
    out[o]=0;
    return o;
}
