/*
 * puttyalt_gui.h: GUI application framework — v2.0 Modern UI.
 *
 * GitHub Dark design, custom-drawn controls, double-buffered rendering.
 */

#ifndef PUTTYALT_GUI_H
#define PUTTYALT_GUI_H

#include "puttyalt.h"

/* Window dimensions */
#define GUI_DEFAULT_WIDTH   960
#define GUI_DEFAULT_HEIGHT  640
#define GUI_MIN_WIDTH       480
#define GUI_MIN_HEIGHT      320

/* Color scheme: Warm Blue */
#define GUI_COLOR_BG          0x1B2838   /* Deep navy */
#define GUI_COLOR_BG_DARK     0x141E2B   /* Darker nav/sidebar */
#define GUI_COLOR_BG_LIGHT    0x233044   /* Card surfaces */
#define GUI_COLOR_SURFACE     0x2A3A50   /* Input fields */
#define GUI_COLOR_BORDER      0x3A4F6A   /* Subtle borders */
#define GUI_COLOR_PRIMARY     0x4A9EE0   /* Warm sky blue */
#define GUI_COLOR_PRIMARY_HOV 0x5DB4F5   /* Hover state */
#define GUI_COLOR_ACCENT      0x64B5F6   /* Light accent */
#define GUI_COLOR_SUCCESS     0x66BB6A   /* Green */
#define GUI_COLOR_WARNING     0xFFA726   /* Orange */
#define GUI_COLOR_ERROR       0xEF5350   /* Red */
#define GUI_COLOR_TEXT        0xE8EDF2   /* Primary text */
#define GUI_COLOR_TEXT_DIM    0x8B9DB8   /* Secondary text */
#define GUI_COLOR_TERMINAL_BG 0x0D1520   /* Terminal bg */
#define GUI_COLOR_TERMINAL_FG 0xD4DEE8   /* Terminal text */
#define GUI_COLOR_CURSOR      0x4A9EE0   /* Cursor */
#define GUI_COLOR_SELECTION   0x2E5C8A   /* Selection highlight */

/* Menu IDs */
#define IDM_FILE_NEW          1001
#define IDM_FILE_OPEN         1002
#define IDM_FILE_SAVE_SESSION 1003
#define IDM_FILE_DUPLICATE    1004
#define IDM_FILE_LOG          1005
#define IDM_FILE_EXPORT       1006
#define IDM_FILE_IMPORT       1007
#define IDM_FILE_EXIT         1008

#define IDM_EDIT_COPY         2001
#define IDM_EDIT_PASTE        2002
#define IDM_EDIT_SELECTALL    2003
#define IDM_EDIT_FIND         2004
#define IDM_EDIT_CLEAR        2005
#define IDM_EDIT_PREFERENCES  2006

#define IDM_SESSION_RECONNECT  3001
#define IDM_SESSION_DISCONNECT 3002
#define IDM_SESSION_SETTINGS   3003
#define IDM_SESSION_SEND_CMD   3004
#define IDM_SESSION_BROADCAST  3005
#define IDM_SESSION_LOCK       3006
#define IDM_SESSION_LOG        3007

#define IDM_VIEW_FULLSCREEN   4001
#define IDM_VIEW_SCROLLBACK   4002
#define IDM_VIEW_TABS         4003
#define IDM_VIEW_SPLIT_H      4004
#define IDM_VIEW_SPLIT_V      4005
#define IDM_VIEW_TOOLBAR      4006
#define IDM_VIEW_STATUSBAR    4007
#define IDM_VIEW_THEME        4008
#define IDM_VIEW_SIDEBAR      4009
#define IDM_VIEW_ZOOM_IN      4010
#define IDM_VIEW_ZOOM_OUT     4011
#define IDM_VIEW_ZOOM_RESET   4012
#define IDM_VIEW_OPACITY      4013

#define IDM_TOOLS_SNIPPETS    5001
#define IDM_TOOLS_MACROS      5002
#define IDM_TOOLS_KEYGEN      5003
#define IDM_TOOLS_SFTP        5004
#define IDM_TOOLS_BROADCAST   5005
#define IDM_TOOLS_BOOKMARKS   5006
#define IDM_TOOLS_TUNNEL      5007
#define IDM_TOOLS_NETDIAG     5008
#define IDM_TOOLS_SCRIPTMGR   5009
#define IDM_TOOLS_CONNPROF    5010
#define IDM_TOOLS_MONITOR     5011
#define IDM_TOOLS_CMDPAL      5012

#define IDM_HELP_ABOUT        6001
#define IDM_HELP_DOCS         6002
#define IDM_HELP_UPDATE       6003
#define IDM_HELP_CHANGELOG    6004
#define IDM_HELP_SHORTCUTS    6005

/* Toolbar button IDs */
#define IDB_CONNECT    101
#define IDB_DISCONNECT 102
#define IDB_NEWTAB     103
#define IDB_SETTINGS   104
#define IDB_SFTP       105
#define IDB_SNIPPETS   106
#define IDB_BROADCAST  107
#define IDB_SPLIT      108
#define IDB_BOOKMARK   109

/* GUI state flags */
#define GUI_STATE_CONNECTED     0x0001
#define GUI_STATE_FULLSCREEN    0x0002
#define GUI_STATE_TOOLBAR_VIS   0x0004
#define GUI_STATE_STATUSBAR_VIS 0x0008
#define GUI_STATE_TAB_BAR_VIS   0x0010
#define GUI_STATE_SPLIT_VIEW    0x0020
#define GUI_STATE_SIDEBAR_VIS   0x0040
#define GUI_STATE_LOCKED        0x0080
#define GUI_STATE_RECORDING     0x0100
#define GUI_STATE_BROADCASTING  0x0200

/* Sidebar panel types */
#define SIDEBAR_SESSIONS  0
#define SIDEBAR_BOOKMARKS 1
#define SIDEBAR_SNIPPETS  2
#define SIDEBAR_SFTP      3
#define SIDEBAR_TUNNELS   4

typedef struct {
    int width;
    int height;
    int x;
    int y;
    int maximized;
    int fullscreen;
    unsigned int state_flags;

    /* Font */
    int font_size;
    char font_name[64];

    /* Appearance */
    char theme_name[64];
    int scrollback_lines;
    int tab_bar_position;  /* 0=top, 1=bottom */
    int toolbar_visible;
    int statusbar_visible;
    int sidebar_visible;
    int sidebar_width;
    int sidebar_panel;
    int transparency;      /* 0-255, 0=opaque */
    int cursor_style;      /* 0=block, 1=underline, 2=bar */
    int cursor_blink;
    int line_spacing;

    /* Behavior */
    int bell_enabled;
    int confirm_on_close;
    int copy_on_select;
    int paste_on_right_click;
    int focus_follows_mouse;
    int scroll_on_output;
    int scroll_on_keypress;
    int word_wrap;
    int bidi_support;

    /* Colors (override theme) */
    unsigned int color_bg;
    unsigned int color_fg;
    unsigned int color_cursor;
    unsigned int color_selection;
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
    int zoom_level;  /* -5 to +5 */
    int sidebar_hover;

#ifdef _WIN32
    void *hwnd;         /* HWND */
    void *hinstance;    /* HINSTANCE */
    void *term_hwnd;    /* Terminal sub-window */
    void *toolbar;      /* Toolbar HWND */
    void *statusbar;    /* Statusbar HWND */
    void *tab_ctrl;     /* Tab control HWND */
    void *sidebar_hwnd; /* Sidebar panel HWND */
    void *font;         /* HFONT */
    void *menu;         /* HMENU */
    void *bg_brush;     /* Background brush */
    void *accel;        /* Accelerator table */
#else
    void *display;
    void *window;
    void *term_widget;
    int fd;
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
void gui_toggle_sidebar(GUIState *gui);
void gui_resize(GUIState *gui, int w, int h);
void gui_update_menu_state(GUIState *gui);
void gui_show_about(GUIState *gui);

/* Zoom */
void gui_zoom_in(GUIState *gui);
void gui_zoom_out(GUIState *gui);
void gui_zoom_reset(GUIState *gui);

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
