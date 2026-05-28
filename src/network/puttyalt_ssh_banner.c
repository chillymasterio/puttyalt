#include <string.h>
#include <stdio.h>
#define BANNER_MAX 4096
static char g_banner[BANNER_MAX]; static int g_banner_len = 0; static int g_show = 1;
void ssh_banner_set(const char *text, int len) {
    int n = len < BANNER_MAX - 1 ? len : BANNER_MAX - 1;
    memcpy(g_banner, text, n); g_banner[n] = 0; g_banner_len = n;
}
const char *ssh_banner_get(void) { return g_banner_len > 0 ? g_banner : NULL; }
void ssh_banner_clear(void) { g_banner_len = 0; g_banner[0] = 0; }
void ssh_banner_set_visible(int visible) { g_show = visible; }
int ssh_banner_visible(void) { return g_show && g_banner_len > 0; }
int ssh_banner_parse_os(const char *banner, char *os, int oslen) {
    if (strstr(banner, "Ubuntu")) snprintf(os, oslen, "Ubuntu");
    else if (strstr(banner, "Debian")) snprintf(os, oslen, "Debian");
    else if (strstr(banner, "CentOS")) snprintf(os, oslen, "CentOS");
    else if (strstr(banner, "OpenSSH")) snprintf(os, oslen, "OpenSSH");
    else snprintf(os, oslen, "Unknown");
    return 0;
}
