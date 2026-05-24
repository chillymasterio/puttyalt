#ifndef PUTTYALT_TEMPLATES_H
#define PUTTYALT_TEMPLATES_H

#define TMPL_MAX 64
#define TMPL_MAX_VARS 16

typedef struct {
    char name[64];
    char host_pattern[128];
    int port;
    char username[64];
    char key_path[256];
    char proxy[128];
    int locked_fields;  /* bitmask: which fields users can't override */
    char vars[TMPL_MAX_VARS][64];
    int var_count;
    char description[256];
    char created_by[32];
    long created_at;
} ConnTemplate;

typedef struct {
    ConnTemplate templates[TMPL_MAX];
    int count;
    int readonly;
    char source_path[512];
} TemplateStore;

int  tmpl_init(TemplateStore *ts);
int  tmpl_add(TemplateStore *ts, const ConnTemplate *t);
int  tmpl_remove(TemplateStore *ts, int idx);
int  tmpl_apply(const TemplateStore *ts, int idx, char *host, int *port,
                char *user, int host_sz, int user_sz);
int  tmpl_load(TemplateStore *ts, const char *path);
int  tmpl_save(const TemplateStore *ts, const char *path);
void tmpl_destroy(TemplateStore *ts);

#endif
