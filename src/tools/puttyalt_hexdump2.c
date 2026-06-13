/* puttyalt_hexdump2.c - Hex dump formatter with offset + ASCII gutter. */
#include <stdio.h>
#include <string.h>
int hexdump2_line(const unsigned char *data, int len, int offset, int width, char *out, int outlen) {
    if(!data||!out) return -1;
    if(width<=0)width=16;
    int pos=snprintf(out,outlen,"%08x  ",offset);
    for (int i=0;i<width;i++) {
        if (i<len) pos+=snprintf(out+pos,outlen-pos,"%02x ",data[i]);
        else pos+=snprintf(out+pos,outlen-pos,"   ");
        if (i==width/2-1) pos+=snprintf(out+pos,outlen-pos," ");
    }
    pos+=snprintf(out+pos,outlen-pos," |");
    for (int i=0;i<len && i<width;i++) {
        unsigned char c=data[i];
        out[pos++]=(c>=32&&c<127)?c:'.';
    }
    out[pos++]='|'; out[pos]=0;
    return pos;
}
int hexdump2_lines_needed(int total, int width) {
    if(width<=0) width=16;
    return (total+width-1)/width;
}
