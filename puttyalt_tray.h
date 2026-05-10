/*
 * puttyalt_tray.h: System tray icon support.
 */

#ifndef PUTTYALT_TRAY_H
#define PUTTYALT_TRAY_H

#include "puttyalt_gui.h"

#define TRAY_MSG_ID    (0x8000 + 1)
#define IDM_TRAY_SHOW  7001
#define IDM_TRAY_HIDE  7002
#define IDM_TRAY_EXIT  7003

typedef struct {
    int visible;
    int minimize_to_tray;
    int close_to_tray;
    int show_notifications;
    char tooltip[128];
#ifdef _WIN32
    void *nid;  /* NOTIFYICONDATA pointer */
#endif
} TrayIcon;

int  tray_init(TrayIcon *tray, GUIState *gui);
void tray_destroy(TrayIcon *tray, GUIState *gui);
void tray_show(TrayIcon *tray, GUIState *gui);
void tray_hide(TrayIcon *tray, GUIState *gui);
void tray_set_tooltip(TrayIcon *tray, GUIState *gui, const char *text);
void tray_notify(TrayIcon *tray, GUIState *gui,
                 const char *title, const char *msg);

#endif /* PUTTYALT_TRAY_H */
