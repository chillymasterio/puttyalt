/* puttyalt_profilediff.c - Connection profile diff (added/changed/removed). */
#include <string.h>
#include <stdio.h>
#define PD_MAX 48
#define PD_KEY 40
#define PD_VAL 96
typedef struct {
    char key[PD_KEY];
    char value[PD_VAL];
} pd_kv;
typedef struct {
    pd_kv items[PD_MAX];
    int n;
} ProfileSet;
void profileset_init(ProfileSet *p) {
    if (p) memset(p, 0, sizeof(*p));
}
int profileset_set(ProfileSet *p, const char *key, const char *value) {
    if (!p || !key) return -1;
    for (int i = 0; i < p->n; i++) {
        if (strcmp(p->items[i].key, key) == 0) {
            snprintf(p->items[i].value, PD_VAL, "%s", value ? value : "");
            return 0;
        }
    }
    if (p->n >= PD_MAX) return -1;
    snprintf(p->items[p->n].key, PD_KEY, "%s", key);
    snprintf(p->items[p->n].value, PD_VAL, "%s", value ? value : "");
    p->n++;
    return 0;
}
static const char *ps_get(const ProfileSet *p, const char *key) {
    for (int i = 0; i < p->n; i++) {
        if (strcmp(p->items[i].key, key) == 0) return p->items[i].value;
    }
    return NULL;
}
int profilediff_compute(const ProfileSet *a, const ProfileSet *b, char *out, int outlen) {
    if (!a || !b || !out) return -1;
    int pos = 0, changes = 0;
    for (int i = 0; i < b->n; i++) {
        const char *av = ps_get(a, b->items[i].key);
        if (!av) {
            pos += snprintf(out + pos, outlen - pos, "+ %s=%s\n", b->items[i].key, b->items[i].value);
            changes++;
        } else if (strcmp(av, b->items[i].value) != 0) {
            pos += snprintf(out + pos, outlen - pos, "~ %s: %s -> %s\n", b->items[i].key, av, b->items[i].value);
            changes++;
        }
    }
    for (int i = 0; i < a->n; i++) {
        if (!ps_get(b, a->items[i].key)) {
            pos += snprintf(out + pos, outlen - pos, "- %s\n", a->items[i].key);
            changes++;
        }
    }
    return changes;
}
