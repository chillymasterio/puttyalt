/* puttyalt_filetype.c - File type detection by extension + magic bytes. */
#include <string.h>
enum ft_type { FT_UNKNOWN=0, FT_TEXT=1, FT_IMAGE=2, FT_ARCHIVE=3, FT_BINARY=4, FT_SCRIPT=5, FT_CONFIG=6 };
int filetype_by_ext(const char *filename) {
    if(!filename) return FT_UNKNOWN;
    const char *dot=strrchr(filename,'.'); if(!dot) return FT_UNKNOWN;
    const char *e=dot+1;
    if (!strcmp(e,"txt")||!strcmp(e,"log")||!strcmp(e,"md")||!strcmp(e,"csv")) return FT_TEXT;
    if (!strcmp(e,"png")||!strcmp(e,"jpg")||!strcmp(e,"gif")||!strcmp(e,"bmp")) return FT_IMAGE;
    if (!strcmp(e,"zip")||!strcmp(e,"tar")||!strcmp(e,"gz")||!strcmp(e,"7z")) return FT_ARCHIVE;
    if (!strcmp(e,"sh")||!strcmp(e,"py")||!strcmp(e,"pl")||!strcmp(e,"rb")) return FT_SCRIPT;
    if (!strcmp(e,"conf")||!strcmp(e,"cfg")||!strcmp(e,"ini")||!strcmp(e,"yaml")||!strcmp(e,"json")) return FT_CONFIG;
    if (!strcmp(e,"exe")||!strcmp(e,"bin")||!strcmp(e,"so")||!strcmp(e,"dll")) return FT_BINARY;
    return FT_UNKNOWN;
}
int filetype_by_magic(const unsigned char *data, int len) {
    if(!data||len<4) return FT_UNKNOWN;
    if (data[0]==0x89&&data[1]=='P'&&data[2]=='N'&&data[3]=='G') return FT_IMAGE;
    if (data[0]==0xFF&&data[1]==0xD8) return FT_IMAGE;
    /* jpeg */
    if (data[0]=='P'&&data[1]=='K') return FT_ARCHIVE;
    /* zip */
    if (data[0]==0x1F&&data[1]==0x8B) return FT_ARCHIVE;
    /* gzip */
    if (data[0]==0x7F&&data[1]=='E'&&data[2]=='L'&&data[3]=='F') return FT_BINARY;
    if (data[0]=='M'&&data[1]=='Z') return FT_BINARY;
    /* PE */
    /* text heuristic: printable */
    int printable=1; for(int i=0;i<len&&i<64;i++) if(data[i]<9||(data[i]>13&&data[i]<32)){printable=0;break;}
    return printable?FT_TEXT:FT_BINARY;
}
