#ifndef PUTTYALT_STATUSBAR_H
#define PUTTYALT_STATUSBAR_H

#define SB_MAX_SECTIONS   16
#define SB_MAX_TEXT       128

typedef enum {
    SB_SECTION_TEXT = 0,
    SB_SECTION_ICON,
    SB_SECTION_PROGRESS,
    SB_SECTION_SEPARATOR
} SBSectionType;

typedef enum {
    SB_ALIGN_LEFT = 0,
    SB_ALIGN_CENTER,
    SB_ALIGN_RIGHT
} SBAlign;

typedef struct {
    SBSectionType type;
    SBAlign       align;
    char          text[SB_MAX_TEXT];
    int           width;       /* pixels, 0 = auto */
    int           color;
    int           bg_color;
    int           icon_id;
    int           progress;    /* 0-100 for progress type */
    int           visible;
    int           clickable;
} SBSection;

typedef struct {
    SBSection sections[SB_MAX_SECTIONS];
    int       count;
    int       height;
    int       bg_color;
    int       fg_color;
    int       visible;
} StatusBar;

void sb_init(StatusBar *sb);
int  sb_add_section(StatusBar *sb, SBSectionType type, SBAlign align, int width);
int  sb_set_text(StatusBar *sb, int index, const char *text);
int  sb_set_progress(StatusBar *sb, int index, int pct);
int  sb_set_color(StatusBar *sb, int index, int fg, int bg);
int  sb_set_icon(StatusBar *sb, int index, int icon_id);
void sb_set_visible(StatusBar *sb, int visible);
void sb_toggle(StatusBar *sb);

/* Convenience: update common status bar fields */
void sb_update_connection(StatusBar *sb, const char *host, int port,
                          const char *protocol);
void sb_update_encoding(StatusBar *sb, const char *encoding);
void sb_update_cursor(StatusBar *sb, int line, int col);
void sb_update_transfer(StatusBar *sb, int pct, const char *filename);

#endif
