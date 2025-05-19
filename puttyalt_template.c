#include <string.h>
#include <stdio.h>
#include "puttyalt_template.h"

void tmpl_init(TemplateRegistry *reg) { memset(reg, 0, sizeof(*reg)); }

int tmpl_create(TemplateRegistry *reg, const char *name, const char *desc)
{
    if (reg->count >= TMPL_MAX) return -1;
    if (tmpl_find(reg, name) >= 0) return -1;
    SessionTemplate *t = &reg->templates[reg->count];
    memset(t, 0, sizeof(*t));
    strncpy(t->name, name, TMPL_NAME_LEN - 1);
    if (desc) strncpy(t->description, desc, TMPL_DESC_LEN - 1);
    t->default_port = 22;
    t->keep_alive_sec = 60;
    return reg->count++;
}

int tmpl_delete(TemplateRegistry *reg, int index)
{
    if (index < 0 || index >= reg->count) return -1;
    for (int i = index; i < reg->count - 1; i++)
        reg->templates[i] = reg->templates[i + 1];
    reg->count--;
    return 0;
}

int tmpl_apply(const TemplateRegistry *reg, int index, void *session_config)
{
    if (index < 0 || index >= reg->count) return -1;
    if (!session_config) return -1;
    (void)session_config;
    return 0;
}

int tmpl_find(const TemplateRegistry *reg, const char *name)
{
    for (int i = 0; i < reg->count; i++)
        if (strcmp(reg->templates[i].name, name) == 0) return i;
    return -1;
}

int tmpl_duplicate(TemplateRegistry *reg, int index, const char *new_name)
{
    if (index < 0 || index >= reg->count) return -1;
    if (reg->count >= TMPL_MAX) return -1;
    if (tmpl_find(reg, new_name) >= 0) return -1;
    reg->templates[reg->count] = reg->templates[index];
    strncpy(reg->templates[reg->count].name, new_name, TMPL_NAME_LEN - 1);
    return reg->count++;
}

int tmpl_save(const TemplateRegistry *reg, const char *path)
{
    FILE *fp = fopen(path, "w");
    if (!fp) return -1;
    for (int i = 0; i < reg->count; i++) {
        const SessionTemplate *t = &reg->templates[i];
        fprintf(fp, "[template:%d]\nname=%s\ndesc=%s\nport=%d\nuser=%s\nkeepalive=%d\n\n",
                i, t->name, t->description, t->default_port, t->default_user, t->keep_alive_sec);
    }
    fclose(fp);
    return 0;
}

int tmpl_load(TemplateRegistry *reg, const char *path)
{
    FILE *fp = fopen(path, "r");
    if (!fp) return -1;
    fclose(fp);
    return 0;
}
