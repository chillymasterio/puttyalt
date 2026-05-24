#ifndef PUTTYALT_TERMINAL_CONF_H
#define PUTTYALT_TERMINAL_CONF_H

#define TCONF_MAX_PROFILES  32
#define TCONF_MAX_NAME      64
#define TCONF_MAX_FONT      128

typedef struct {
    char  name[TCONF_MAX_NAME];
    char  font_name[TCONF_MAX_FONT];
    int   font_size;
    int   cols;
    int   rows;
    int   scrollback;
    int   cursor_type;       /* 0=block, 1=underline, 2=bar */
    int   cursor_blink;
    int   bell_enabled;
    int   bell_visual;
    int   word_chars[256];   /* char class for double-click selection */
    int   auto_wrap;
    int   auto_scroll;
    int   mouse_reporting;
    int   bracketed_paste;
    int   alt_screen;
    int   utf8;
    int   answerback_len;
    char  answerback[64];
} TermProfile;

typedef struct {
    TermProfile profiles[TCONF_MAX_PROFILES];
    int         count;
    int         active;
} TermConfMgr;

void tconf_init(TermConfMgr *tc);
int  tconf_add(TermConfMgr *tc, const char *name);
int  tconf_remove(TermConfMgr *tc, int index);
int  tconf_activate(TermConfMgr *tc, int index);
int  tconf_find(const TermConfMgr *tc, const char *name);
int  tconf_duplicate(TermConfMgr *tc, int index, const char *new_name);
int  tconf_load(TermConfMgr *tc, const char *path);
int  tconf_save(const TermConfMgr *tc, const char *path);
void tconf_install_defaults(TermConfMgr *tc);

#endif
