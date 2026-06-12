/* puttyalt_sesstemplate2.c - Session template v2 with variable substitution. */
#include <string.h>
#include <stdio.h>
#define ST_VARS 16
#define ST_NAME 32
#define ST_VAL 96
typedef struct {
    char key[ST_NAME];
    char value[ST_VAL];
} st_var;
typedef struct {
    char template_str[256];
    st_var vars[ST_VARS];
    int nvars;
} SessTemplate2;
void sesstemplate2_init(SessTemplate2 *s, const char *tmpl) {
    if (!s) return;
    memset(s, 0, sizeof(*s));
    snprintf(s->template_str, 256, "%s", tmpl ? tmpl : "");
}
int sesstemplate2_set(SessTemplate2 *s, const char *key, const char *value) {
    if (!s || !key) return -1;
    for (int i = 0; i < s->nvars; i++) {
        if (strcmp(s->vars[i].key, key) == 0) {
            snprintf(s->vars[i].value, ST_VAL, "%s", value ? value : "");
            return 0;
        }
    }
    if (s->nvars >= ST_VARS) return -1;
    snprintf(s->vars[s->nvars].key, ST_NAME, "%s", key);
    snprintf(s->vars[s->nvars].value, ST_VAL, "%s", value ? value : "");
    s->nvars++;
    return 0;
}
int sesstemplate2_render(const SessTemplate2 *s, char *out, int outlen) {
    if (!s || !out) return -1;
    int o = 0;
    const char *p = s->template_str;
    while (*p && o < outlen - 1) {
        if (p[0] == '{' && p[1] == '{') {
            const char *end = strstr(p + 2, "}}");
            if (end) {
                char key[ST_NAME];
                int kl = (int)(end - (p + 2));
                if (kl >= ST_NAME) kl = ST_NAME - 1;
                memcpy(key, p + 2, kl);
                key[kl] = 0;
                for (int i = 0; i < s->nvars; i++) {
                    if (strcmp(s->vars[i].key, key) == 0) {
                        o += snprintf(out + o, outlen - o, "%s", s->vars[i].value);
                        break;
                    }
                }
                p = end + 2;
                continue;
            }
        }
        out[o++] = *p++;
    }
    out[o] = 0;
    return o;
}
int sesstemplate2_var_count(const SessTemplate2 *s) {
    return s ? s->nvars : -1;
}
