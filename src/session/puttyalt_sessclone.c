#include <string.h>
#include <stdio.h>
typedef struct { char host[256]; int port; char user[64]; char keyfile[256]; char label[32]; int color_scheme; int font_size; } SessionConfig;
int sessclone_copy(const SessionConfig *src, SessionConfig *dst) {
    memcpy(dst, src, sizeof(SessionConfig)); return 0;
}
int sessclone_diff(const SessionConfig *a, const SessionConfig *b, char *out, int outlen) {
    int pos = 0;
    if (strcmp(a->host, b->host) != 0) pos += snprintf(out+pos, outlen-pos, "host: %s -> %s\n", a->host, b->host);
    if (a->port != b->port) pos += snprintf(out+pos, outlen-pos, "port: %d -> %d\n", a->port, b->port);
    if (strcmp(a->user, b->user) != 0) pos += snprintf(out+pos, outlen-pos, "user: %s -> %s\n", a->user, b->user);
    if (a->font_size != b->font_size) pos += snprintf(out+pos, outlen-pos, "font: %d -> %d\n", a->font_size, b->font_size);
    return pos;
}
int sessclone_template(const SessionConfig *tmpl, const char *new_host, SessionConfig *out) {
    memcpy(out, tmpl, sizeof(SessionConfig)); snprintf(out->host, 256, "%s", new_host); return 0;
}
