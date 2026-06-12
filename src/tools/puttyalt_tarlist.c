/* puttyalt_tarlist.c - TAR archive entry header parser (ustar). */
#include <string.h>
#include <stdio.h>
typedef struct { char name[100]; long size; int type; int mode; } TarEntry;
static long tar_octal(const char *p, int len) {
    long v=0; for (int i=0;i<len && p[i];i++) { if(p[i]>='0'&&p[i]<='7') v=v*8+(p[i]-'0'); }
    return v;
}
int tarlist_parse_header(const unsigned char *block, TarEntry *out) {
    if(!block||!out) return -1;
    memset(out,0,sizeof(*out));
    /* check for empty block (end of archive) */
    int empty=1; for(int i=0;i<512;i++) if(block[i]){ empty=0; break; }
    if (empty) return 1;
    /* ustar magic at 257 */
    if (memcmp(block+257,"ustar",5)!=0) return -1;
    memcpy(out->name,block,99); out->name[99]=0;
    out->size=tar_octal((const char*)block+124,12);
    out->mode=(int)tar_octal((const char*)block+100,8);
    out->type=block[156]?block[156]-'0':0;
    return 0;
}
int tarlist_is_dir(const TarEntry *e) { return (e&&e->type==5)?1:0; }
int tarlist_blocks_for(long size) { return (int)((size+511)/512); }
