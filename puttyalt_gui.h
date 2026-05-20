/*
 * puttyalt_gui.h: GUI application framework.
 */

#ifndef PUTTYALT_GUI_H
#define PUTTYALT_GUI_H

#include "puttyalt.h"

/* Window dimensions */
#define GUI_DEFAULT_WIDTH   800
#define GUI_DEFAULT_HEIGHT  600
#define GUI_MIN_WIDTH       400
#define GUI_MIN_HEIGHT      300

/* Menu IDs */
#define IDM_FILE_NEW          1001
#define IDM_FILE_OPEN         1002
#define IDM_FILE_SAVE_SESSION 1003
#define IDM_FILE_DUPLICATE    1004
#define IDM_FILE_LOG          1005
#define IDM_FILE_EXIT         1006

#define IDM_EDIT_COPY         2001
#define IDM_EDIT_PASTE        2002
#define IDM_EDIT_SELECTALL    2003
#define IDM_EDIT_FIND         2004
#define IDM_EDIT_CLEAR        2005

#define IDM_SESSION_RECONNECT 3001
#define IDM_SESSION_DISCONNECT 3002
#define IDM_SESSION_SETTINGS  3003
#define IDM_SESSION_SEND_CMD  3004

#define IDM_VIEW_FULLSCREEN   4001
#define IDM_VIEW_SCROLLBACK   4002
#define IDM_VIEW_TABS         4003
#define IDM_VIEW_SPLIT_H      4004
#define IDM_VIEW_SPLIT_V      4005
#define IDM_VIEW_TOOLBAR      4006
#define IDM_VIEW_STATUSBAR    4007
#define IDM_VIEW_THEME        4008

#define IDM_TOOLS_SNIPPETS    5001
#define IDM_TOOLS_MACROS      5002
#define IDM_TOOLS_KEYGEN      5003
#define IDM_TOOLS_SFTP        5004
#define IDM_TOOLS_BROADCAST   5005
#define IDM_TOOLS_BOOKMARKS   5006
#define IDM_TOOLS_TUNNEL      5007
#define IDM_TOOLS_NETDIAG     5008

#define IDM_HELP_ABOUT        6001
#define IDM_HELP_DOCS         6002
#define IDM_HELP_UPDATE       6003

/* Toolbar button IDs */
#define IDB_CONNECT    101
#define IDB_DISCONNECT 102
#define IDB_NEWTAB     103
#define IDB_SETTINGS   104
#define IDB_SFTP       105
#define IDB_SNIPPETS   106

/* GUI state flags */
#define GUI_STATE_CONNECTED    0x01
#define GUI_STATE_FULLSCREEN   0x02
#define GUI_STATE_TOOLBAR_VIS  0x04
#define GUI_STATE_STATUSBAR_VIS 0x08
#define GUI_STATE_TAB_BAR_VIS  0x10
#define GUI_STATE_SPLIT_VIEW   0x20

typedef struct {
    int width;
    int height;
    int x;
    int y;
    int maximized;
    int fullscreen;
    unsigned int state_flags;
    int font_size;
    char font_name[64];
    char theme_name[64];
    int scrollback_lines;
    int tab_bar_position;  /* 0=top, 1=bottom */
    int toolbar_visible;
    int statusbar_visible;
    int transparency;      /* 0-255, 0=opaque */
    int bell_enabled;
    int confirm_on_close;
} GUIConfig;

typedef struct {
    GUIConfig config;
    int running;
    int connected;
    int num_sessions;
    int active_session;
    char title[256];
    char status_text[256];
    unsigned long bytes_sent;
    unsigned long bytes_recv;
    unsigned long uptime_start;

#ifdef _WIN32
    void *hwnd;         /* HWND */
    void *hinstance;    /* HINSTANCE */
    void *term_hwnd;    /* Terminal sub-window */
    void *toolbar;      /* Toolbar HWND */
    void *statusbar;    /* Statusbar HWND */
    void *tab_ctrl;     /* Tab control HWND */
    void *font;         /* HFONT */
    void *menu;         /* HMENU */
#else
    void *display;      /* X11 Display or GTK */
    void *window;
    void *term_widget;
    int fd;             /* PTY master fd */
#endif
} GUIState;

/* Core GUI lifecycle */
int  gui_init(GUIState *gui, void *instance);
void gui_destroy(GUIState *gui);
int  gui_run(GUIState *gui);
void gui_quit(GUIState *gui);

/* Configuration */
void gui_config_defaults(GUIConfig *cfg);
int  gui_config_load(GUIConfig *cfg, const char *path);
int  gui_config_save(const GUIConfig *cfg, const char *path);

/* Window management */
void gui_set_title(GUIState *gui, const char *title);
void gui_set_status(GUIState *gui, const char *text);
void gui_toggle_fullscreen(GUIState *gui);
void gui_resize(GUIState *gui, int w, int h);
void gui_update_menu_state(GUIState *gui);
void gui_show_about(GUIState *gui);

/* Terminal operations */
int  gui_terminal_write(GUIState *gui, const unsigned char *data, int len);
void gui_terminal_clear(GUIState *gui);
void gui_terminal_set_font(GUIState *gui, const char *name, int size);
void gui_terminal_scroll(GUIState *gui, int lines);

/* Connection */
int  gui_connect(GUIState *gui, const char *host, int port, const char *user);
void gui_disconnect(GUIState *gui);
int  gui_reconnect(GUIState *gui);

/* Dialog helpers */
int  gui_confirm(GUIState *gui, const char *title, const char *msg);
void gui_error(GUIState *gui, const char *title, const char *msg);
char *gui_input_dialog(GUIState *gui, const char *title, const char *prompt);

#endif /* PUTTYALT_GUI_H */
