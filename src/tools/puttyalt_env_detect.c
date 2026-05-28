#include <string.h>
#include <stdio.h>
typedef struct { char os[64]; char shell[64]; char term[64]; char user[64]; char hostname[256]; int color_support; } RemoteEnv;
int envdetect_parse_prompt(const char *prompt, RemoteEnv *env) {
    memset(env, 0, sizeof(*env));
    const char *at = strchr(prompt, '@');
    if (at) { int ulen = (int)(at - prompt); if (ulen > 63) ulen = 63; memcpy(env->user, prompt, ulen); snprintf(env->hostname, 256, "%s", at + 1); }
    return 0;
}
int envdetect_from_uname(const char *uname, RemoteEnv *env) {
    if (strstr(uname, "Linux")) snprintf(env->os, 64, "Linux");
    else if (strstr(uname, "Darwin")) snprintf(env->os, 64, "macOS");
    else if (strstr(uname, "FreeBSD")) snprintf(env->os, 64, "FreeBSD");
    else snprintf(env->os, 64, "Unknown");
    return 0;
}
int envdetect_color_depth(const char *term) {
    if (strstr(term, "256color")) return 256;
    if (strstr(term, "truecolor") || strstr(term, "24bit")) return 16777216;
    return 16;
}
