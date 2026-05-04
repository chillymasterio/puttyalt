/*
 * puttyalt_statusbar.c: Status bar implementation.
 */

#include "puttyalt_statusbar.h"
#include <string.h>
#include <stdio.h>

void statusbar_init(StatusBar *sb)
{
    memset(sb, 0, sizeof(*sb));
    sb->visible = 1;
    sb->height_px = 22;
}

void statusbar_set(StatusBar *sb, StatusBarSection section, const char *text)
{
    if (section >= 0 && section < STATUSBAR_MAX_SECTIONS && text)
        snprintf(sb->sections[section], STATUSBAR_SECTION_LEN, "%s", text);
}

void statusbar_clear(StatusBar *sb)
{
    for (int i = 0; i < STATUSBAR_MAX_SECTIONS; i++)
        sb->sections[i][0] = '\0';
}

void statusbar_toggle(StatusBar *sb)
{
    sb->visible = !sb->visible;
}

const char *statusbar_get(const StatusBar *sb, StatusBarSection section)
{
    if (section >= 0 && section < STATUSBAR_MAX_SECTIONS)
        return sb->sections[section];
    return "";
}
