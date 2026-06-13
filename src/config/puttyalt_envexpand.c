/* puttyalt_envexpand.c - Environment variable expansion ($VAR and ${VAR}). */
#include <string.h>
#include <stdio.h>
#define EE_MAX 48
#define EE_NAME 40
#define EE_VAL 128
typedef struct {
    char name[EE_NAME];
    char value[EE_VAL];
} ee_var;
typedef struct {
    ee_var vars[EE_MAX];
    int n;
} EnvExpand;
void envexpand_init(EnvExpand *e) {
    if (e) memset(e, 0, sizeof(*e));
}
int envexpand_set(EnvExpand *e, const char *name, const char *value) {
    if (!e || !name) return -1;
    for (int i = 0; i < e->n; i++) {
        if (strcmp(e->vars[i].name, name) == 0) {
            snprintf(e->vars[i].value, EE_VAL, "%s", value ? value : "");
            return 0;
        }
    }
    if (e->n >= EE_MAX) return -1;
    snprintf(e->vars[e->n].name, EE_NAME, "%s", name);
    snprintf(e->vars[e->n].value, EE_VAL, "%s", value ? value : "");
    e->n++;
    return 0;
}
static const char *ee_lookup(const EnvExpand *e, const char *name, int len) {
    for (int i = 0; i < e->n; i++) {
        if ((int)strlen(e->vars[i].name) == len && strncmp(e->vars[i].name, name, len) == 0) {
            return e->vars[i].value;
        }
    }
    return NULL;
}
int envexpand_expand(const EnvExpand *e, const char *in, char *out, int outlen) {
    if (!e || !in || !out) return -1;
    int o = 0;
    const char *p = in;
    while (*p && o < outlen - 1) {
        if (*p == '$') {
            const char *name;
            int len;
            if (p[1] == '{') {
                const char *end = strchr(p + 2, '}');
                if (end) {
                    name = p + 2;
                    len = (int)(end - name);
                    const char *v = ee_lookup(e, name, len);
                    if (v) o += snprintf(out + o, outlen - o, "%s", v);
                    p = end + 1;
                    continue;
                }
            } else {
                name = p + 1;
                len = 0;
                while (name[len] && (name[len] == '_' ||
                       (name[len] >= 'A' && name[len] <= 'Z') ||
                       (name[len] >= 'a' && name[len] <= 'z') ||
                       (name[len] >= '0' && name[len] <= '9'))) len++;
                if (len > 0) {
                    const char *v = ee_lookup(e, name, len);
                    if (v) o += snprintf(out + o, outlen - o, "%s", v);
                    p = name + len;
                    continue;
                }
            }
        }
        out[o++] = *p++;
    }
    out[o] = 0;
    return o;
}
int envexpand_count(const EnvExpand *e) {
    return e ? e->n : -1;
}
