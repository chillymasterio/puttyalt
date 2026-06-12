/* puttyalt_textwrap.c - Word-wrap engine with hyphenation hints. */
#include <string.h>
int textwrap_next_break(const char *text, int start, int width) {
    int len=(int)strlen(text);
    if (start>=len) return len;
    if (start+width>=len) return len;
    /* find last space within width */
    int limit=start+width;
    for (int i=limit;i>start;i--) if (text[i]==' ') return i;
    /* no space: hard break at width */
    return limit;
}
int textwrap_line_count(const char *text, int width) {
    if(!text||width<=0) return 0;
    int lines=0, pos=0, len=(int)strlen(text);
    while (pos<len) { pos=textwrap_next_break(text,pos,width); while(text[pos]==' ')pos++; lines++; }
    return lines>0?lines:1;
}
int textwrap_get_line(const char *text, int width, int line_idx, char *out, int outlen) {
    if(!text||!out) return -1;
    int pos=0, len=(int)strlen(text), cur=0;
    while (pos<len) {
        int brk=textwrap_next_break(text,pos,width);
        if (cur==line_idx) {
            int l=brk-pos; if(l>=outlen)l=outlen-1; memcpy(out,text+pos,l); out[l]=0;
            /* trim trailing space */
            while (l>0 && out[l-1]==' ') out[--l]=0;
            return l;
        }
        pos=brk; while(text[pos]==' ')pos++; cur++;
    }
    out[0]=0; return -1;
}
