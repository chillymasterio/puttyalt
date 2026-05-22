#include "puttyalt_templates.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int tmpl_init(TemplateStore *ts)
{
    memset(ts, 0, sizeof(*ts));
    return 0;
}

int tmpl_add(TemplateStore *ts, const ConnTemplate *t)
{
    if (ts->count >= TMPL_MAX || ts->readonly) return -1;
    ts->templates[ts->count] = *t;
    if (!t->created_at)
        ts->templates[ts->count].created_at = (long)time(NULL);
    ts->count++;
    return ts->count - 1;
}

int tmpl_remove(TemplateStore *ts, int idx)
{
    if (idx < 0 || idx >= ts->count || ts->readonly) return -1;
    for (int i = idx; i < ts->count - 1; i++)
        ts->templates[i] = ts->templates[i + 1];
    ts->count--;
    return 0;
}

int tmpl_apply(const TemplateStore *ts, int idx, char *host, int *port,
               char *user, int host_sz, int user_sz)
{
    if (idx < 0 || idx >= ts->count) return -1;
    const ConnTemplate *t = &ts->templates[idx];
    if (t->host_pattern[0])
        snprintf(host, host_sz, "%s", t->host_pattern);
    if (t->port > 0) *port = t->port;
    if (t->username[0])
        snprintf(user, user_sz, "%s", t->username);
    return 0;
}

int tmpl_load(TemplateStore *ts, const char *path)
{
    FILE *f = fopen(path, "r");
    if (!f) return -1;
    char line[1024];
    int cur = -1;
    while (fgets(line, sizeof(line), f)) {
        size_t len = strlen(line);
        while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r'))
            line[--len] = '\0';
        if (line[0] == '[' && line[len-1] == ']') {
            line[len-1] = '\0';
            if (ts->count < TMPL_MAX) {
                cur = ts->count;
                memset(&ts->templates[cur], 0, sizeof(ConnTemplate));
                snprintf(ts->templates[cur].name, 64, "%s", line + 1);
                ts->count++;
            }
        } else if (cur >= 0) {
            char *eq = strchr(line, '=');
            if (!eq) continue;
            *eq++ = '\0';
            ConnTemplate *t = &ts->templates[cur];
            if (strcmp(line, "host") == 0) snprintf(t->host_pattern, 128, "%s", eq);
            else if (strcmp(line, "port") == 0) t->port = atoi(eq);
            else if (strcmp(line, "user") == 0) snprintf(t->username, 64, "%s", eq);
            else if (strcmp(line, "key") == 0) snprintf(t->key_path, 256, "%s", eq);
            else if (strcmp(line, "desc") == 0) snprintf(t->description, 256, "%s", eq);
        }
    }
    fclose(f);
    snprintf(ts->source_path, sizeof(ts->source_path), "%s", path);
    return 0;
}

int tmpl_save(const TemplateStore *ts, const char *path)
{
    FILE *f = fopen(path, "w");
    if (!f) return -1;
    for (int i = 0; i < ts->count; i++) {
        const ConnTemplate *t = &ts->templates[i];
        fprintf(f, "[%s]\n", t->name);
        if (t->host_pattern[0]) fprintf(f, "host=%s\n", t->host_pattern);
        if (t->port) fprintf(f, "port=%d\n", t->port);
        if (t->username[0]) fprintf(f, "user=%s\n", t->username);
        if (t->key_path[0]) fprintf(f, "key=%s\n", t->key_path);
        if (t->description[0]) fprintf(f, "desc=%s\n", t->description);
        fprintf(f, "\n");
    }
    fclose(f);
    return 0;
}

void tmpl_destroy(TemplateStore *ts) { memset(ts, 0, sizeof(*ts)); }
