/*
 * puttyalt_dialogs.h: Settings and connection dialogs.
 */

#ifndef PUTTYALT_DIALOGS_H
#define PUTTYALT_DIALOGS_H

#include "puttyalt_gui.h"

/* Dialog IDs */
#define IDD_CONNECT       200
#define IDD_SETTINGS      201
#define IDD_ABOUT         202
#define IDD_SNIPPET_MGR   203
#define IDD_KEYGEN        204
#define IDD_TUNNEL_MGR    205
#define IDD_BOOKMARK_MGR  206
#define IDD_THEME_SELECT  207
#define IDD_FIND          208

/* Connect dialog controls */
#define IDC_HOST      1101
#define IDC_PORT      1102
#define IDC_USER      1103
#define IDC_PASS      1104
#define IDC_PROTOCOL  1105
#define IDC_KEYFILE   1106
#define IDC_SAVE_SESS 1107
#define IDC_SESS_NAME 1108
#define IDC_SESS_LIST 1109
#define IDC_CONNECT   1110
#define IDC_BROWSE    1111

/* Settings dialog controls */
#define IDC_FONT_NAME   1201
#define IDC_FONT_SIZE   1202
#define IDC_SCROLLBACK  1203
#define IDC_TRANSPARENCY 1204
#define IDC_BELL_CHECK  1205
#define IDC_CONFIRM_CHK 1206
#define IDC_TAB_POS     1207
#define IDC_THEME_LIST  1208

/* Connection parameters */
typedef struct {
    char hostname[256];
    int port;
    char username[128];
    char password[128];
    char keyfile[512];
    int protocol;  /* 0=SSH, 1=Telnet, 2=Serial, 3=Raw */
    int save_session;
    char session_name[128];
} ConnectParams;

/* Dialog functions */
int  dialog_connect(GUIState *gui, ConnectParams *params);
int  dialog_settings(GUIState *gui);
int  dialog_snippet_manager(GUIState *gui);
int  dialog_keygen(GUIState *gui);
int  dialog_tunnel_manager(GUIState *gui);
int  dialog_bookmark_manager(GUIState *gui);
int  dialog_theme_select(GUIState *gui);
int  dialog_find(GUIState *gui, char *search_text, int max_len);

/* Quick-connect from command line args */
int  parse_connect_args(int argc, char **argv, ConnectParams *params);

#endif /* PUTTYALT_DIALOGS_H */
