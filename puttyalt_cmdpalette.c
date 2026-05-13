#include "puttyalt_cmdpalette.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void cmdpal_init(CmdPalette *cp) { memset(cp, 0, sizeof(*cp)); cp->selected = -1; }

int cmdpal_register(CmdPalette *cp, const char *name, const char *desc,
                    const char *category, const char *shortcut, int id)
{
    if (cp->count >= CP_MAX_COMMANDS) return -1;
    CPCommand *c = &cp->commands[cp->count];
    memset(c, 0, sizeof(*c));
    snprintf(c->name, CP_MAX_NAME, "%s", name);
    if (desc) snprintf(c->description, CP_MAX_DESC, "%s", desc);
    if (category) snprintf(c->category, sizeof(c->category), "%s", category);
    if (shortcut) snprintf(c->shortcut, sizeof(c->shortcut), "%s", shortcut);
    c->id = id;
    return cp->count++;
}

static int fuzzy_match(const char *str, const char *query)
{
    int score = 0;
    const char *s = str, *q = query;
    while (*s && *q) {
        if (tolower((unsigned char)*s) == tolower((unsigned char)*q)) {
            score += (s == str || *(s-1) == ' ' || *(s-1) == '.') ? 10 : 1;
            q++;
        }
        s++;
    }
    return *q ? 0 : score;
}

int cmdpal_search(CmdPalette *cp, const char *query)
{
    snprintf(cp->query, CP_MAX_NAME, "%s", query);
    cp->result_count = 0;
    cp->selected = -1;
    if (!query || !*query) {
        /* Show all non-hidden, sorted by use_count */
        for (int i = 0; i < cp->count && cp->result_count < CP_MAX_RESULTS; i++)
            if (!cp->commands[i].hidden)
                cp->results[cp->result_count++] = cp->commands[i];
    } else {
        /* Fuzzy match + sort by score */
        int scores[CP_MAX_COMMANDS];
        for (int i = 0; i < cp->count; i++) {
            if (cp->commands[i].hidden) { scores[i] = 0; continue; }
            scores[i] = fuzzy_match(cp->commands[i].name, query);
            int ds = fuzzy_match(cp->commands[i].description, query);
            if (ds > scores[i]) scores[i] = ds;
        }
        for (int n = 0; n < CP_MAX_RESULTS; n++) {
            int best = -1, best_score = 0;
            for (int i = 0; i < cp->count; i++) {
                if (scores[i] > best_score) { best = i; best_score = scores[i]; }
            }
            if (best < 0) break;
            cp->results[cp->result_count++] = cp->commands[best];
            scores[best] = 0;
        }
    }
    if (cp->result_count > 0) cp->selected = 0;
    return cp->result_count;
}

int cmdpal_execute(CmdPalette *cp)
{
    if (cp->selected < 0 || cp->selected >= cp->result_count) return -1;
    cp->results[cp->selected].use_count++;
    /* Update the original command's use_count */
    int id = cp->results[cp->selected].id;
    for (int i = 0; i < cp->count; i++)
        if (cp->commands[i].id == id) { cp->commands[i].use_count++; break; }
    cp->visible = 0;
    return id;
}

void cmdpal_show(CmdPalette *cp) { cp->visible = 1; cp->query[0] = '\0'; cmdpal_search(cp, ""); }
void cmdpal_hide(CmdPalette *cp) { cp->visible = 0; }
void cmdpal_select_next(CmdPalette *cp) { if (cp->result_count) cp->selected = (cp->selected+1)%cp->result_count; }
void cmdpal_select_prev(CmdPalette *cp) { if (cp->result_count) cp->selected = (cp->selected-1+cp->result_count)%cp->result_count; }

void cmdpal_load_defaults(CmdPalette *cp)
{
    cmdpal_register(cp, "New Connection", "Open new SSH connection", "Session", "Ctrl+N", 100);
    cmdpal_register(cp, "New Tab", "Open new terminal tab", "Tab", "Ctrl+Shift+T", 101);
    cmdpal_register(cp, "Close Tab", "Close current tab", "Tab", "Ctrl+Shift+W", 102);
    cmdpal_register(cp, "Split Horizontal", "Split pane horizontally", "View", "Ctrl+Alt+H", 103);
    cmdpal_register(cp, "Split Vertical", "Split pane vertically", "View", "Ctrl+Alt+V", 104);
    cmdpal_register(cp, "Toggle Sidebar", "Show/hide sidebar panel", "View", "Ctrl+B", 105);
    cmdpal_register(cp, "Find", "Search in terminal output", "Edit", "Ctrl+F", 106);
    cmdpal_register(cp, "Copy", "Copy selection to clipboard", "Edit", "Ctrl+Shift+C", 107);
    cmdpal_register(cp, "Paste", "Paste from clipboard", "Edit", "Ctrl+Shift+V", 108);
    cmdpal_register(cp, "Zoom In", "Increase font size", "View", "Ctrl++", 109);
    cmdpal_register(cp, "Zoom Out", "Decrease font size", "View", "Ctrl+-", 110);
    cmdpal_register(cp, "Reset Zoom", "Reset to default font size", "View", "Ctrl+0", 111);
    cmdpal_register(cp, "Preferences", "Open preferences dialog", "Settings", "", 112);
    cmdpal_register(cp, "Color Scheme", "Change color scheme", "Settings", "", 113);
    cmdpal_register(cp, "Session Manager", "Manage saved sessions", "Session", "", 114);
    cmdpal_register(cp, "SFTP Panel", "Toggle SFTP file panel", "Tools", "", 115);
    cmdpal_register(cp, "Port Forwarding", "Manage port forwarding rules", "Tools", "", 116);
    cmdpal_register(cp, "Protocol Analyzer", "Open protocol analyzer", "Tools", "", 117);
    cmdpal_register(cp, "Debug Console", "Toggle debug console", "Tools", "", 118);
    cmdpal_register(cp, "About", "Show version information", "Help", "", 119);
}
