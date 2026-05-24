#include "puttyalt_statusbar.h"
#include <stdio.h>
#include <string.h>

void sb_init(StatusBar *sb)
{
    memset(sb, 0, sizeof(*sb));
    sb->height = 22;
    sb->bg_color = 0x14202E;
    sb->fg_color = 0x8899AA;
    sb->visible = 1;

    /* Default sections: connection | encoding | cursor | transfer | clock */
    sb_add_section(sb, SB_SECTION_TEXT, SB_ALIGN_LEFT, 200);   /* connection */
    sb_add_section(sb, SB_SECTION_SEPARATOR, SB_ALIGN_LEFT, 1);
    sb_add_section(sb, SB_SECTION_TEXT, SB_ALIGN_LEFT, 80);    /* encoding */
    sb_add_section(sb, SB_SECTION_SEPARATOR, SB_ALIGN_LEFT, 1);
    sb_add_section(sb, SB_SECTION_TEXT, SB_ALIGN_RIGHT, 100);  /* cursor pos */
    sb_add_section(sb, SB_SECTION_PROGRESS, SB_ALIGN_RIGHT, 120); /* transfer */
    sb_add_section(sb, SB_SECTION_TEXT, SB_ALIGN_RIGHT, 60);   /* clock */

    sb_set_text(sb, 0, "Disconnected");
    sb_set_text(sb, 2, "UTF-8");
    sb_set_text(sb, 4, "Ln 1, Col 1");
}

int sb_add_section(StatusBar *sb, SBSectionType type, SBAlign align, int width)
{
    if (sb->count >= SB_MAX_SECTIONS) return -1;
    SBSection *s = &sb->sections[sb->count];
    memset(s, 0, sizeof(*s));
    s->type = type;
    s->align = align;
    s->width = width;
    s->visible = 1;
    s->color = sb->fg_color;
    s->bg_color = sb->bg_color;
    return sb->count++;
}

int sb_set_text(StatusBar *sb, int index, const char *text)
{
    if (index < 0 || index >= sb->count) return -1;
    snprintf(sb->sections[index].text, SB_MAX_TEXT, "%s", text);
    return 0;
}

int sb_set_progress(StatusBar *sb, int index, int pct)
{
    if (index < 0 || index >= sb->count) return -1;
    if (pct < 0) pct = 0;
    if (pct > 100) pct = 100;
    sb->sections[index].progress = pct;
    return 0;
}

int sb_set_color(StatusBar *sb, int index, int fg, int bg)
{
    if (index < 0 || index >= sb->count) return -1;
    sb->sections[index].color = fg;
    sb->sections[index].bg_color = bg;
    return 0;
}

int sb_set_icon(StatusBar *sb, int index, int icon_id)
{
    if (index < 0 || index >= sb->count) return -1;
    sb->sections[index].icon_id = icon_id;
    return 0;
}

void sb_set_visible(StatusBar *sb, int visible)
{
    sb->visible = visible;
}

void sb_toggle(StatusBar *sb)
{
    sb->visible = !sb->visible;
}

void sb_update_connection(StatusBar *sb, const char *host, int port,
                          const char *protocol)
{
    char buf[SB_MAX_TEXT];
    snprintf(buf, sizeof(buf), "%s %s:%d", protocol, host, port);
    sb_set_text(sb, 0, buf);
    sb_set_color(sb, 0, 0x2ECC71, sb->bg_color); /* green when connected */
}

void sb_update_encoding(StatusBar *sb, const char *encoding)
{
    sb_set_text(sb, 2, encoding);
}

void sb_update_cursor(StatusBar *sb, int line, int col)
{
    char buf[64];
    snprintf(buf, sizeof(buf), "Ln %d, Col %d", line, col);
    sb_set_text(sb, 4, buf);
}

void sb_update_transfer(StatusBar *sb, int pct, const char *filename)
{
    sb_set_progress(sb, 5, pct);
    if (filename)
        snprintf(sb->sections[5].text, SB_MAX_TEXT, "%s (%d%%)", filename, pct);
    else
        sb->sections[5].text[0] = '\0';
}
