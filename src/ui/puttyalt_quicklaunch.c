#include "puttyalt_quicklaunch.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void qlbar_init(QuickLaunchBar *ql)
{
    memset(ql, 0, sizeof(*ql));
    ql->visible = 1;
}

int qlbar_add(QuickLaunchBar *ql, const QuickLaunchItem *item)
{
    if (ql->count >= QL_MAX_ITEMS) return -1;
    if (item->name[0] == '\0') return -1;
    memcpy(&ql->items[ql->count], item, sizeof(QuickLaunchItem));
    ql->items[ql->count].position = ql->count;
    ql->count++;
    return 0;
}

int qlbar_remove(QuickLaunchBar *ql, const char *name)
{
    for (int i = 0; i < ql->count; i++) {
        if (strcmp(ql->items[i].name, name) == 0) {
            for (int j = i; j < ql->count - 1; j++)
                ql->items[j] = ql->items[j + 1];
            ql->count--;
            return 0;
        }
    }
    return -1;
}

int qlbar_activate(QuickLaunchBar *ql, int index)
{
    if (index < 0 || index >= ql->count) return -1;
    ql->items[index].uses++;
    return 0;
}

int qlbar_move(QuickLaunchBar *ql, int from, int to)
{
    if (from < 0 || from >= ql->count || to < 0 || to >= ql->count) return -1;
    QuickLaunchItem tmp = ql->items[from];
    if (from < to)
        for (int i = from; i < to; i++) ql->items[i] = ql->items[i + 1];
    else
        for (int i = from; i > to; i--) ql->items[i] = ql->items[i - 1];
    ql->items[to] = tmp;
    return 0;
}

int qlbar_save(const QuickLaunchBar *ql, const char *path)
{
    FILE *f = fopen(path, "w");
    if (!f) return -1;
    for (int i = 0; i < ql->count; i++) {
        const QuickLaunchItem *it = &ql->items[i];
        fprintf(f, "[item]\nname=%s\ntarget=%s\ntype=%d\nshortcut=%s\n\n",
                it->name, it->target, it->type, it->shortcut);
    }
    fclose(f);
    return 0;
}

int qlbar_load(QuickLaunchBar *ql, const char *path)
{
    FILE *f = fopen(path, "r");
    char line[1024];
    if (!f) return -1;
    QuickLaunchItem *cur = NULL;
    while (fgets(line, sizeof(line), f)) {
        size_t len = strlen(line);
        while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r'))
            line[--len] = '\0';
        if (strcmp(line, "[item]") == 0) {
            if (ql->count >= QL_MAX_ITEMS) break;
            cur = &ql->items[ql->count++];
            memset(cur, 0, sizeof(*cur));
            continue;
        }
        if (!cur) continue;
        if (strncmp(line, "name=", 5) == 0)
            snprintf(cur->name, sizeof(cur->name), "%s", line + 5);
        else if (strncmp(line, "target=", 7) == 0)
            snprintf(cur->target, sizeof(cur->target), "%s", line + 7);
        else if (strncmp(line, "type=", 5) == 0) cur->type = atoi(line + 5);
        else if (strncmp(line, "shortcut=", 9) == 0)
            snprintf(cur->shortcut, sizeof(cur->shortcut), "%s", line + 9);
    }
    fclose(f);
    return 0;
}
