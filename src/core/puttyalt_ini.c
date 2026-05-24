#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "puttyalt_ini.h"

static char *trim(char *s)
{
    while (isspace((unsigned char)*s)) s++;
    char *end = s + strlen(s) - 1;
    while (end > s && isspace((unsigned char)*end)) *end-- = '\0';
    return s;
}

static INISection *find_section(INIFile *ini, const char *name)
{
    for (int i = 0; i < ini->num_sections; i++)
        if (strcmp(ini->sections[i].name, name) == 0)
            return &ini->sections[i];
    return NULL;
}

static INISection *get_or_create_section(INIFile *ini, const char *name)
{
    INISection *s = find_section(ini, name);
    if (s) return s;
    if (ini->num_sections >= INI_MAX_SECTIONS) return NULL;
    s = &ini->sections[ini->num_sections++];
    memset(s, 0, sizeof(*s));
    strncpy(s->name, name, INI_SECTION_LEN - 1);
    return s;
}

int ini_load(INIFile *ini, const char *path)
{
    memset(ini, 0, sizeof(*ini));
    strncpy(ini->path, path, sizeof(ini->path) - 1);

    FILE *fp = fopen(path, "r");
    if (!fp) return -1;

    char line[1024];
    INISection *cur = NULL;

    while (fgets(line, sizeof(line), fp)) {
        char *p = trim(line);
        if (*p == '\0' || *p == '#' || *p == ';') continue;

        if (*p == '[') {
            char *end = strchr(p, ']');
            if (end) {
                *end = '\0';
                cur = get_or_create_section(ini, p + 1);
            }
        } else if (cur) {
            char *eq = strchr(p, '=');
            if (eq && cur->count < INI_MAX_KEYS) {
                *eq = '\0';
                INIEntry *e = &cur->entries[cur->count++];
                strncpy(e->key, trim(p), INI_KEY_LEN - 1);
                strncpy(e->value, trim(eq + 1), INI_VAL_LEN - 1);
            }
        }
    }
    fclose(fp);
    return 0;
}

int ini_save(const INIFile *ini)
{
    FILE *fp = fopen(ini->path, "w");
    if (!fp) return -1;
    for (int i = 0; i < ini->num_sections; i++) {
        const INISection *s = &ini->sections[i];
        fprintf(fp, "[%s]\n", s->name);
        for (int j = 0; j < s->count; j++)
            fprintf(fp, "%s=%s\n", s->entries[j].key, s->entries[j].value);
        fprintf(fp, "\n");
    }
    fclose(fp);
    return 0;
}

const char *ini_get(const INIFile *ini, const char *section, const char *key)
{
    const INISection *s = NULL;
    for (int i = 0; i < ini->num_sections; i++)
        if (strcmp(ini->sections[i].name, section) == 0)
            { s = &ini->sections[i]; break; }
    if (!s) return NULL;
    for (int j = 0; j < s->count; j++)
        if (strcmp(s->entries[j].key, key) == 0)
            return s->entries[j].value;
    return NULL;
}

int ini_get_int(const INIFile *ini, const char *section, const char *key, int def)
{
    const char *val = ini_get(ini, section, key);
    return val ? atoi(val) : def;
}

int ini_set(INIFile *ini, const char *section, const char *key, const char *value)
{
    INISection *s = get_or_create_section(ini, section);
    if (!s) return -1;
    for (int j = 0; j < s->count; j++) {
        if (strcmp(s->entries[j].key, key) == 0) {
            strncpy(s->entries[j].value, value, INI_VAL_LEN - 1);
            ini->modified = 1;
            return 0;
        }
    }
    if (s->count >= INI_MAX_KEYS) return -1;
    INIEntry *e = &s->entries[s->count++];
    strncpy(e->key, key, INI_KEY_LEN - 1);
    strncpy(e->value, value, INI_VAL_LEN - 1);
    ini->modified = 1;
    return 0;
}

int ini_set_int(INIFile *ini, const char *section, const char *key, int value)
{
    char buf[32];
    snprintf(buf, sizeof(buf), "%d", value);
    return ini_set(ini, section, key, buf);
}

int ini_delete_key(INIFile *ini, const char *section, const char *key)
{
    INISection *s = find_section(ini, section);
    if (!s) return -1;
    for (int j = 0; j < s->count; j++) {
        if (strcmp(s->entries[j].key, key) == 0) {
            for (int k = j; k < s->count - 1; k++)
                s->entries[k] = s->entries[k + 1];
            s->count--;
            ini->modified = 1;
            return 0;
        }
    }
    return -1;
}

int ini_delete_section(INIFile *ini, const char *section)
{
    for (int i = 0; i < ini->num_sections; i++) {
        if (strcmp(ini->sections[i].name, section) == 0) {
            for (int j = i; j < ini->num_sections - 1; j++)
                ini->sections[j] = ini->sections[j + 1];
            ini->num_sections--;
            ini->modified = 1;
            return 0;
        }
    }
    return -1;
}
