#include "puttyalt_snippetlib.h"
#include <string.h>
#include <stdio.h>

void snippetlib_init(SnippetLibrary *sl) { memset(sl, 0, sizeof(*sl)); }

static int find_or_add_cat(SnippetLibrary *sl, const char *cat)
{
    for (int i = 0; i < sl->cat_count; i++)
        if (strcmp(sl->categories[i], cat) == 0) return i;
    if (sl->cat_count < SNIPPET_CAT_MAX)
        snprintf(sl->categories[sl->cat_count++], 32, "%s", cat);
    return sl->cat_count - 1;
}

int snippetlib_add(SnippetLibrary *sl, const char *name, const char *cmd, const char *cat)
{
    if (sl->count >= SNIPPET_MAX) return -1;
    Snippet *s = &sl->items[sl->count];
    memset(s, 0, sizeof(*s));
    snprintf(s->name, sizeof(s->name), "%s", name);
    snprintf(s->command, sizeof(s->command), "%s", cmd);
    snprintf(s->category, sizeof(s->category), "%s", cat ? cat : "General");
    find_or_add_cat(sl, s->category);
    sl->count++;
    return sl->count - 1;
}

int snippetlib_remove(SnippetLibrary *sl, int index)
{
    if (index < 0 || index >= sl->count) return -1;
    memmove(&sl->items[index], &sl->items[index+1], (sl->count - index - 1) * sizeof(Snippet));
    sl->count--;
    return 0;
}

int snippetlib_find(SnippetLibrary *sl, const char *name)
{
    for (int i = 0; i < sl->count; i++)
        if (strcmp(sl->items[i].name, name) == 0) return i;
    return -1;
}

void snippetlib_sort_by_usage(SnippetLibrary *sl)
{
    /* simple insertion sort by usage_count desc */
    for (int i = 1; i < sl->count; i++) {
        Snippet tmp = sl->items[i];
        int j = i - 1;
        while (j >= 0 && sl->items[j].usage_count < tmp.usage_count) {
            sl->items[j+1] = sl->items[j];
            j--;
        }
        sl->items[j+1] = tmp;
    }
}

int snippetlib_search(SnippetLibrary *sl, const char *query, int *results, int max)
{
    int n = 0;
    for (int i = 0; i < sl->count && n < max; i++) {
        /* simple substring match on name, command, category */
        if (strstr(sl->items[i].name, query) ||
            strstr(sl->items[i].command, query) ||
            strstr(sl->items[i].category, query))
            results[n++] = i;
    }
    return n;
}

int snippetlib_save(SnippetLibrary *sl, const char *path)
{
    FILE *f = fopen(path, "w");
    if (!f) return -1;
    for (int i = 0; i < sl->count; i++) {
        Snippet *s = &sl->items[i];
        fprintf(f, "[%s]\ncmd=%s\ncat=%s\ndesc=%s\nkey=%s\nuses=%d\npin=%d\n\n",
                s->name, s->command, s->category, s->description,
                s->shortcut, s->usage_count, s->pinned);
    }
    fclose(f);
    return 0;
}

int snippetlib_load(SnippetLibrary *sl, const char *path)
{
    FILE *f = fopen(path, "r");
    if (!f) return -1;
    /* simplified parser */
    char line[1024], name[64] = {0}, cmd[1024] = {0}, cat[32] = {0};
    while (fgets(line, sizeof(line), f)) {
        if (line[0] == '[') {
            if (name[0] && cmd[0]) snippetlib_add(sl, name, cmd, cat);
            char *end = strchr(line, ']');
            if (end) { *end = '\0'; snprintf(name, sizeof(name), "%s", line+1); }
            cmd[0] = cat[0] = '\0';
        } else if (strncmp(line, "cmd=", 4) == 0) {
            snprintf(cmd, sizeof(cmd), "%s", line+4);
            cmd[strcspn(cmd, "\n")] = '\0';
        } else if (strncmp(line, "cat=", 4) == 0) {
            snprintf(cat, sizeof(cat), "%s", line+4);
            cat[strcspn(cat, "\n")] = '\0';
        }
    }
    if (name[0] && cmd[0]) snippetlib_add(sl, name, cmd, cat);
    fclose(f);
    return 0;
}

void snippetlib_add_defaults(SnippetLibrary *sl)
{
    snippetlib_add(sl, "Disk usage", "df -h", "System");
    snippetlib_add(sl, "Memory info", "free -m", "System");
    snippetlib_add(sl, "Process list", "ps aux --sort=-%mem | head -20", "System");
    snippetlib_add(sl, "Network connections", "ss -tunap", "Network");
    snippetlib_add(sl, "Find large files", "find / -type f -size +100M 2>/dev/null", "Files");
    snippetlib_add(sl, "Tail syslog", "tail -f /var/log/syslog", "Logs");
    snippetlib_add(sl, "Docker PS", "docker ps --format 'table {{.Names}}\t{{.Status}}\t{{.Ports}}'", "Docker");
    snippetlib_add(sl, "Git status", "git status -sb", "Development");
    snippetlib_add(sl, "System uptime", "uptime", "System");
    snippetlib_add(sl, "IP addresses", "ip -4 addr show | grep inet", "Network");
}
