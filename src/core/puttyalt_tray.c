/*
 * puttyalt_tray.c: System tray icon state management.
 * Tray icon rendering is handled by the GUI framework (gui_wndproc).
 * This module manages tray state and configuration.
 */

#include "puttyalt_tray.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int tray_init(TrayIcon *tray, GUIState *gui)
{
    (void)gui;
    memset(tray, 0, sizeof(*tray));
    tray->minimize_to_tray = 1;
    tray->close_to_tray = 0;
    tray->show_notifications = 1;
    snprintf(tray->tooltip, sizeof(tray->tooltip),
             "%s %s", PUTTYALT_NAME, PUTTYALT_VERSION_STR);
    tray->visible = 1;
    return 0;
}

void tray_destroy(TrayIcon *tray, GUIState *gui)
{
    (void)gui;
#ifdef _WIN32
    if (tray->nid) {
        free(tray->nid);
        tray->nid = NULL;
    }
#endif
    tray->visible = 0;
}

void tray_show(TrayIcon *tray, GUIState *gui)
{
    (void)gui;
    tray->visible = 1;
}

void tray_hide(TrayIcon *tray, GUIState *gui)
{
    (void)gui;
    tray->visible = 0;
}

void tray_set_tooltip(TrayIcon *tray, GUIState *gui, const char *text)
{
    (void)gui;
    snprintf(tray->tooltip, sizeof(tray->tooltip), "%s", text);
}

void tray_notify(TrayIcon *tray, GUIState *gui,
                 const char *title, const char *msg)
{
    (void)gui; (void)title; (void)msg;
    if (!tray->show_notifications) return;
    /* Notification display handled by GUI event loop */
}
