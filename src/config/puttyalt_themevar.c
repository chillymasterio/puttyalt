/* puttyalt_themevar.c - Theme variable resolver with CSS-like cascading. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define TV_MAX 48
#define TV_NAME 40
typedef struct {
    char name[TV_NAME];
    char value[40];
    int is_reference;
} tv_var;
typedef struct {
    tv_var vars[TV_MAX];
    int n;
} ThemeVar;
void themevar_init(ThemeVar *t) {
    if (t) memset(t, 0, sizeof(*t));
}
int themevar_set(ThemeVar *t, const char *name, const char *value) {
    if (!t || !name) return -1;
    for (int i = 0; i < t->n; i++) {
        if (strcmp(t->vars[i].name, name) == 0) {
            snprintf(t->vars[i].value, 40, "%s", value ? value : "");
            t->vars[i].is_reference = (value && value[0] == '$');
            return 0;
        }
    }
    if (t->n >= TV_MAX) return -1;
    tv_var *v = &t->vars[t->n++];
    snprintf(v->name, TV_NAME, "%s", name);
    snprintf(v->value, 40, "%s", value ? value : "");
    v->is_reference = (value && value[0] == '$');
    return 0;
}
const char *themevar_resolve(const ThemeVar *t, const char *name) {
    if (!t || !name) return 0;
    const char *cur = name;
    for (int depth = 0; depth < TV_MAX; depth++) {
        const char *found = 0;
        for (int i = 0; i < t->n; i++) {
            if (strcmp(t->vars[i].name, cur) == 0) {
                found = t->vars[i].value;
                break;
            }
        }
        if (!found) return 0;
        if (found[0] == '$') {
            cur = found + 1;
            continue;
        }
        return found;
    }
    return 0;
}
int themevar_count(const ThemeVar *t) {
    return t ? t->n : -1;
}
