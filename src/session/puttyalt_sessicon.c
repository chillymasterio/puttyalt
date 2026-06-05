/* puttyalt_sessicon.c - Session icon/badge assignment by OS + role detection. */
#include <string.h>
enum si_os { SI_UNKNOWN=0, SI_LINUX=1, SI_WINDOWS=2, SI_MACOS=3, SI_BSD=4, SI_ROUTER=5, SI_CONTAINER=6 };
int sessicon_detect_os(const char *banner) {
    if(!banner) return SI_UNKNOWN;
    if (strstr(banner,"Ubuntu")||strstr(banner,"Debian")||strstr(banner,"Linux")) return SI_LINUX;
    if (strstr(banner,"Windows")||strstr(banner,"Microsoft")) return SI_WINDOWS;
    if (strstr(banner,"Darwin")||strstr(banner,"macOS")) return SI_MACOS;
    if (strstr(banner,"FreeBSD")||strstr(banner,"OpenBSD")||strstr(banner,"NetBSD")) return SI_BSD;
    if (strstr(banner,"RouterOS")||strstr(banner,"Cisco")||strstr(banner,"JUNOS")) return SI_ROUTER;
    if (strstr(banner,"docker")||strstr(banner,"alpine")||strstr(banner,"k8s")) return SI_CONTAINER;
    return SI_UNKNOWN;
}
const char *sessicon_glyph_name(int os) {
    static const char *n[]={"server","linux","windows","apple","bsd","router","container"};
    return (os>=0&&os<=SI_CONTAINER)?n[os]:"server";
}
unsigned int sessicon_accent(int os) {
    static const unsigned int c[]={0x888888,0xE95420,0x0078D4,0xA2AAAD,0xAB2B28,0x1BA0E2,0x2496ED};
    return (os>=0&&os<=SI_CONTAINER)?c[os]:0x888888;
}
