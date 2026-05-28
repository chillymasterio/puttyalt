#include <string.h>
#include <stdio.h>
static int g_truecolor = 1;
void truecolor_enable(int on) { g_truecolor = on; }
int truecolor_enabled(void) { return g_truecolor; }
int truecolor_parse_sgr(const char *seq, unsigned int *fg, unsigned int *bg) {
    *fg = 0; *bg = 0;
    const char *p = seq;
    while (*p) {
        if (strncmp(p, "38;2;", 5) == 0) { int r,g,b; sscanf(p+5, "%d;%d;%d", &r, &g, &b); *fg = (r<<16)|(g<<8)|b; }
        if (strncmp(p, "48;2;", 5) == 0) { int r,g,b; sscanf(p+5, "%d;%d;%d", &r, &g, &b); *bg = (r<<16)|(g<<8)|b; }
        p++;
    }
    return (*fg || *bg) ? 0 : -1;
}
int truecolor_format_fg(unsigned int color, char *buf, int buflen) {
    return snprintf(buf, buflen, "\x1b[38;2;%d;%d;%dm", (color>>16)&0xFF, (color>>8)&0xFF, color&0xFF);
}
int truecolor_format_bg(unsigned int color, char *buf, int buflen) {
    return snprintf(buf, buflen, "\x1b[48;2;%d;%d;%dm", (color>>16)&0xFF, (color>>8)&0xFF, color&0xFF);
}
