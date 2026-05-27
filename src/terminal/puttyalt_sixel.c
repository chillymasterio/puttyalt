#include <string.h>
#include <stdio.h>
typedef struct { int width; int height; int colors; unsigned char *data; int data_len; } SixelImage;
int sixel_detect(const unsigned char *buf, int len) {
    for (int i=0;i<len-2;i++) if (buf[i]==0x1b && buf[i+1]=='P' && buf[i+2]=='q') return i;
    return -1;
}
int sixel_parse_header(const unsigned char *buf, int len, int *width, int *height) {
    *width=0; *height=0;
    for (int i=0;i<len;i++) {
        if (buf[i]=='"') { int a=0,b=0,c=0,d=0;
            sscanf((const char*)buf+i+1,"%d;%d;%d;%d",&a,&b,&c,&d);
            *width=c; *height=d; return 0;
        }
    }
    return -1;
}
int sixel_supported(void) { return 1; }
