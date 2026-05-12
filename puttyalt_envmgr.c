#include "puttyalt_envmgr.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void envmgr_init(EnvMgr *em)
{
    memset(em, 0, sizeof(*em));
    em->active = -1;
}

int envmgr_add_profile(EnvMgr *em, const char *name)
{
    if (em->profile_count >= ENV_MAX_PROFILES) return -1;
    EnvProfile *p = &em->profiles[em->profile_count];
    memset(p, 0, sizeof(*p));
    snprintf(p->name, ENV_MAX_PNAME, "%s", name);
    return em->profile_count++;
}

int envmgr_remove_profile(EnvMgr *em, int index)
{
    if (index < 0 || index >= em->profile_count) return -1;
    for (int i = index; i < em->profile_count - 1; i++)
        em->profiles[i] = em->profiles[i + 1];
    em->profile_count--;
    if (em->active == index) em->active = -1;
    else if (em->active > index) em->active--;
    return 0;
}

int envmgr_set_active(EnvMgr *em, int index)
{
    if (index < 0 || index >= em->profile_count) return -1;
    em->active = index;
    return 0;
}

static int find_var(const EnvProfile *p, const char *name)
{
    for (int i = 0; i < p->count; i++)
        if (strcmp(p->vars[i].name, name) == 0) return i;
    return -1;
}

int envmgr_set_var(EnvMgr *em, int profile, const char *name,
                   const char *value, int sensitive)
{
    if (profile < 0 || profile >= em->profile_count) return -1;
    EnvProfile *p = &em->profiles[profile];
    int idx = find_var(p, name);
    if (idx >= 0) {
        snprintf(p->vars[idx].value, ENV_MAX_VALUE, "%s", value);
        p->vars[idx].sensitive = sensitive;
        return idx;
    }
    if (p->count >= ENV_MAX_VARS) return -1;
    EnvVar *v = &p->vars[p->count];
    snprintf(v->name, ENV_MAX_NAME, "%s", name);
    snprintf(v->value, ENV_MAX_VALUE, "%s", value);
    v->exported = 1;
    v->sensitive = sensitive;
    return p->count++;
}

int envmgr_unset_var(EnvMgr *em, int profile, const char *name)
{
    if (profile < 0 || profile >= em->profile_count) return -1;
    EnvProfile *p = &em->profiles[profile];
    int idx = find_var(p, name);
    if (idx < 0) return -1;
    for (int i = idx; i < p->count - 1; i++)
        p->vars[i] = p->vars[i + 1];
    p->count--;
    return 0;
}

const char *envmgr_get_var(const EnvMgr *em, int profile, const char *name)
{
    if (profile < 0 || profile >= em->profile_count) return NULL;
    int idx = find_var(&em->profiles[profile], name);
    if (idx < 0) return NULL;
    return em->profiles[profile].vars[idx].value;
}

int envmgr_export(const EnvMgr *em, int profile, char *buf, int bufsz)
{
    if (profile < 0 || profile >= em->profile_count) return -1;
    const EnvProfile *p = &em->profiles[profile];
    int pos = 0;
    for (int i = 0; i < p->count && pos < bufsz - 1; i++) {
        if (p->vars[i].exported)
            pos += snprintf(buf + pos, bufsz - pos, "export %s=\"%s\"\n",
                           p->vars[i].name, p->vars[i].value);
    }
    return pos;
}

int envmgr_load(EnvMgr *em, const char *path)
{
    FILE *f = fopen(path, "r");
    char line[1024];
    if (!f) return -1;
    envmgr_init(em);
    EnvProfile *cur = NULL;
    while (fgets(line, sizeof(line), f)) {
        size_t len = strlen(line);
        while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r'))
            line[--len] = '\0';
        if (strncmp(line, "[profile:", 9) == 0) {
            char *end = strchr(line + 9, ']');
            if (end) *end = '\0';
            int idx = envmgr_add_profile(em, line + 9);
            cur = idx >= 0 ? &em->profiles[idx] : NULL;
        } else if (cur && strchr(line, '=')) {
            char *eq = strchr(line, '=');
            *eq = '\0';
            envmgr_set_var(em, (int)(cur - em->profiles), line, eq + 1, 0);
        }
    }
    fclose(f);
    return 0;
}

int envmgr_save(const EnvMgr *em, const char *path)
{
    FILE *f = fopen(path, "w");
    if (!f) return -1;
    for (int i = 0; i < em->profile_count; i++) {
        const EnvProfile *p = &em->profiles[i];
        fprintf(f, "[profile:%s]\n", p->name);
        for (int j = 0; j < p->count; j++)
            fprintf(f, "%s=%s\n", p->vars[j].name, p->vars[j].value);
        fprintf(f, "\n");
    }
    fclose(f);
    return 0;
}
