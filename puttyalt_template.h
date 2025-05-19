#ifndef PUTTYALT_TEMPLATE_H
#define PUTTYALT_TEMPLATE_H

#define TMPL_MAX          32
#define TMPL_NAME_LEN     64
#define TMPL_DESC_LEN     256

typedef struct SessionTemplate {
    char name[TMPL_NAME_LEN];
    char description[TMPL_DESC_LEN];
    char default_user[128];
    int default_port;
    int color_scheme;
    int font_size;
    int auto_reconnect;
    int enable_logging;
    int keep_alive_sec;
    char startup_cmd[256];
} SessionTemplate;

typedef struct TemplateRegistry {
    SessionTemplate templates[TMPL_MAX];
    int count;
} TemplateRegistry;

void tmpl_init(TemplateRegistry *reg);
int  tmpl_create(TemplateRegistry *reg, const char *name, const char *desc);
int  tmpl_delete(TemplateRegistry *reg, int index);
int  tmpl_apply(const TemplateRegistry *reg, int index, void *session_config);
int  tmpl_find(const TemplateRegistry *reg, const char *name);
int  tmpl_duplicate(TemplateRegistry *reg, int index, const char *new_name);
int  tmpl_save(const TemplateRegistry *reg, const char *path);
int  tmpl_load(TemplateRegistry *reg, const char *path);

#endif
