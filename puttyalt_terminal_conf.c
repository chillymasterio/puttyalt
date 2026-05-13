#include "puttyalt_terminal_conf.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void tconf_init(TermConfMgr *tc)
{
    memset(tc, 0, sizeof(*tc));
    tconf_install_defaults(tc);
}

static void tconf_set_defaults(TermProfile *p)
{
    snprintf(p->font_name, TCONF_MAX_FONT, "Cascadia Code");
    p->font_size = 11;
    p->cols = 80;
    p->rows = 24;
    p->scrollback = 10000;
    p->cursor_type = 0;
    p->cursor_blink = 1;
    p->bell_enabled = 1;
    p->auto_wrap = 1;
    p->auto_scroll = 1;
    p->mouse_reporting = 1;
    p->bracketed_paste = 1;
    p->alt_screen = 1;
    p->utf8 = 1;
}

int tconf_add(TermConfMgr *tc, const char *name)
{
    if (tc->count >= TCONF_MAX_PROFILES) return -1;
    TermProfile *p = &tc->profiles[tc->count];
    memset(p, 0, sizeof(*p));
    snprintf(p->name, TCONF_MAX_NAME, "%s", name);
    tconf_set_defaults(p);
    return tc->count++;
}

int tconf_remove(TermConfMgr *tc, int index)
{
    if (index < 0 || index >= tc->count || tc->count <= 1) return -1;
    for (int i = index; i < tc->count - 1; i++)
        tc->profiles[i] = tc->profiles[i + 1];
    tc->count--;
    if (tc->active >= tc->count) tc->active = tc->count - 1;
    return 0;
}

int tconf_activate(TermConfMgr *tc, int index)
{
    if (index < 0 || index >= tc->count) return -1;
    tc->active = index;
    return 0;
}

int tconf_find(const TermConfMgr *tc, const char *name)
{
    for (int i = 0; i < tc->count; i++)
        if (strcmp(tc->profiles[i].name, name) == 0) return i;
    return -1;
}

int tconf_duplicate(TermConfMgr *tc, int index, const char *new_name)
{
    if (index < 0 || index >= tc->count) return -1;
    if (tc->count >= TCONF_MAX_PROFILES) return -1;
    tc->profiles[tc->count] = tc->profiles[index];
    snprintf(tc->profiles[tc->count].name, TCONF_MAX_NAME, "%s", new_name);
    return tc->count++;
}

int tconf_load(TermConfMgr *tc, const char *path)
{
    FILE *f = fopen(path, "r");
    char line[256];
    TermProfile *cur = NULL;
    if (!f) return -1;
    tc->count = 0;
    while (fgets(line, sizeof(line), f)) {
        size_t len = strlen(line);
        while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r'))
            line[--len] = '\0';
        if (strcmp(line, "[profile]") == 0) {
            int idx = tconf_add(tc, "");
            if (idx < 0) break;
            cur = &tc->profiles[idx];
            continue;
        }
        if (!cur) continue;
        if (strncmp(line, "name=", 5) == 0)
            snprintf(cur->name, TCONF_MAX_NAME, "%s", line + 5);
        else if (strncmp(line, "font=", 5) == 0)
            snprintf(cur->font_name, TCONF_MAX_FONT, "%s", line + 5);
        else if (strncmp(line, "size=", 5) == 0)
            cur->font_size = atoi(line + 5);
        else if (strncmp(line, "cols=", 5) == 0)
            cur->cols = atoi(line + 5);
        else if (strncmp(line, "rows=", 5) == 0)
            cur->rows = atoi(line + 5);
        else if (strncmp(line, "scrollback=", 11) == 0)
            cur->scrollback = atoi(line + 11);
    }
    fclose(f);
    return 0;
}

int tconf_save(const TermConfMgr *tc, const char *path)
{
    FILE *f = fopen(path, "w");
    if (!f) return -1;
    for (int i = 0; i < tc->count; i++) {
        const TermProfile *p = &tc->profiles[i];
        fprintf(f, "[profile]\nname=%s\nfont=%s\nsize=%d\ncols=%d\nrows=%d\n"
                   "scrollback=%d\ncursor=%d\nblink=%d\nutf8=%d\n\n",
                p->name, p->font_name, p->font_size, p->cols, p->rows,
                p->scrollback, p->cursor_type, p->cursor_blink, p->utf8);
    }
    fclose(f);
    return 0;
}

void tconf_install_defaults(TermConfMgr *tc)
{
    int idx = tconf_add(tc, "Default");
    if (idx >= 0) tc->active = idx;

    idx = tconf_add(tc, "Compact");
    if (idx >= 0) {
        tc->profiles[idx].cols = 80;
        tc->profiles[idx].rows = 24;
        tc->profiles[idx].font_size = 10;
        tc->profiles[idx].scrollback = 5000;
    }

    idx = tconf_add(tc, "Widescreen");
    if (idx >= 0) {
        tc->profiles[idx].cols = 200;
        tc->profiles[idx].rows = 50;
        tc->profiles[idx].font_size = 10;
        tc->profiles[idx].scrollback = 50000;
    }

    idx = tconf_add(tc, "Presentation");
    if (idx >= 0) {
        tc->profiles[idx].cols = 80;
        tc->profiles[idx].rows = 24;
        tc->profiles[idx].font_size = 18;
        tc->profiles[idx].scrollback = 1000;
        tc->profiles[idx].cursor_blink = 0;
    }
}
