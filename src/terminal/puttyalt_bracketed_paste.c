#include <string.h>
#include <stdio.h>
static int g_enabled = 0;
void bracketed_paste_set(int on) { g_enabled = on; }
int bracketed_paste_enabled(void) { return g_enabled; }
int bracketed_paste_wrap(const char *text, char *out, int outlen) {
    if (!g_enabled) { snprintf(out, outlen, "%s", text); return (int)strlen(text); }
    return snprintf(out, outlen, "\x1b[200~%s\x1b[201~", text);
}
int bracketed_paste_detect(const unsigned char *buf, int len) {
    if (len >= 6 && buf[0]==0x1b && buf[1]=='[' && buf[2]=='?' && buf[3]=='2' && buf[4]=='0' && buf[5]=='0')
        return buf[6] == 'h' ? 1 : (buf[6] == 'l' ? -1 : 0);
    return 0;
}
