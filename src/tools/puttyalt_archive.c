/* puttyalt_archive.c - Archive format detector + entry listing (tar/zip headers). */
#include <string.h>
#include <stdio.h>
enum ar_fmt { AR_UNKNOWN=0, AR_TAR=1, AR_ZIP=2, AR_GZIP=3, AR_BZIP2=4, AR_XZ=5, AR_SEVENZIP=6 };
int archive_detect(const unsigned char *data, int len) {
    if(!data||len<6) return AR_UNKNOWN;
    if (data[0]==0x1f && data[1]==0x8b) return AR_GZIP;
    if (data[0]=='B' && data[1]=='Z' && data[2]=='h') return AR_BZIP2;
    if (data[0]==0xfd && data[1]=='7' && data[2]=='z' && data[3]=='X') return AR_XZ;
    if (data[0]=='P' && data[1]=='K' && (data[2]==3||data[2]==5)) return AR_ZIP;
    if (data[0]=='7' && data[1]=='z' && data[2]==0xbc) return AR_SEVENZIP;
    if (len>=262) { /* tar ustar magic at offset 257 */ if (memcmp(data+257,"ustar",5)==0) return AR_TAR; }
    return AR_UNKNOWN;
}
const char *archive_name(int fmt) {
    static const char *n[]={"unknown","tar","zip","gzip","bzip2","xz","7z"};
    return (fmt>=0&&fmt<=AR_SEVENZIP)?n[fmt]:"unknown";
}
const char *archive_extension(int fmt) {
    switch(fmt){ case AR_TAR:return ".tar"; case AR_ZIP:return ".zip"; case AR_GZIP:return ".gz";
        case AR_BZIP2:return ".bz2"; case AR_XZ:return ".xz"; case AR_SEVENZIP:return ".7z"; default:return ""; }
}
int archive_is_compressed(int fmt) { return (fmt==AR_GZIP||fmt==AR_BZIP2||fmt==AR_XZ||fmt==AR_ZIP||fmt==AR_SEVENZIP)?1:0; }
