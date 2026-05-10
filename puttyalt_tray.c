/*
 * puttyalt_tray.c: System tray icon implementation.
 */

#include "puttyalt_tray.h"
#include <string.h>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#include <shellapi.h>

int tray_init(TrayIcon *tray, GUIState *gui)
{
    memset(tray, 0, sizeof(*tray));
    tray->minimize_to_tray = 1;
    tray->close_to_tray = 0;
    tray->show_notifications = 1;

    snprintf(tray->tooltip, sizeof(tray->tooltip),
             "%s %s", PUTTYALT_NAME, PUTTYALT_VERSION_STR);

    NOTIFYICONDATAA *nid = (NOTIFYICONDATAA *)calloc(1, sizeof(*nid));
    if (!nid) return -1;

    nid->cbSize = sizeof(*nid);
    nid->hWnd = (HWND)gui->hwnd;
    nid->uID = 1;
    nid->uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid->uCallbackMessage = TRAY_MSG_ID;
    nid->hIcon = LoadIcon(NULL, IDI_APPLICATION);
    strncpy(nid->szTip, tray->tooltip, sizeof(nid->szTip) - 1);

    Shell_NotifyIconA(NIM_ADD, nid);
    tray->nid = nid;
    tray->visible = 1;

    return 0;
}

void tray_destroy(TrayIcon *tray, GUIState *gui)
{
    (void)gui;
    if (tray->nid) {
        Shell_NotifyIconA(NIM_DELETE, (NOTIFYICONDATAA *)tray->nid);
        free(tray->nid);
        tray->nid = NULL;
    }
    tray->visible = 0;
}

void tray_show(TrayIcon *tray, GUIState *gui)
{
    if (!tray->visible && tray->nid) {
        Shell_NotifyIconA(NIM_ADD, (NOTIFYICONDATAA *)tray->nid);
        tray->visible = 1;
    }
    (void)gui;
}

void tray_hide(TrayIcon *tray, GUIState *gui)
{
    if (tray->visible && tray->nid) {
        Shell_NotifyIconA(NIM_DELETE, (NOTIFYICONDATAA *)tray->nid);
        tray->visible = 0;
    }
    (void)gui;
}

void tray_set_tooltip(TrayIcon *tray, GUIState *gui, const char *text)
{
    (void)gui;
    snprintf(tray->tooltip, sizeof(tray->tooltip), "%s", text);
    if (tray->nid) {
        NOTIFYICONDATAA *nid = (NOTIFYICONDATAA *)tray->nid;
        strncpy(nid->szTip, text, sizeof(nid->szTip) - 1);
        nid->uFlags = NIF_TIP;
        Shell_NotifyIconA(NIM_MODIFY, nid);
    }
}

void tray_notify(TrayIcon *tray, GUIState *gui,
                 const char *title, const char *msg)
{
    (void)gui;
    if (!tray->show_notifications || !tray->nid) return;

    NOTIFYICONDATAA *nid = (NOTIFYICONDATAA *)tray->nid;
    nid->uFlags = NIF_INFO;
    nid->dwInfoFlags = NIIF_INFO;
    strncpy(nid->szInfoTitle, title, sizeof(nid->szInfoTitle) - 1);
    strncpy(nid->szInfo, msg, sizeof(nid->szInfo) - 1);
    Shell_NotifyIconA(NIM_MODIFY, nid);
}

#else /* Unix stubs */

int tray_init(TrayIcon *tray, GUIState *gui)
{
    (void)gui;
    memset(tray, 0, sizeof(*tray));
    tray->minimize_to_tray = 1;
    snprintf(tray->tooltip, sizeof(tray->tooltip),
             "%s %s", PUTTYALT_NAME, PUTTYALT_VERSION_STR);
    return 0;
}

void tray_destroy(TrayIcon *tray, GUIState *gui)
{
    (void)gui; tray->visible = 0;
}
void tray_show(TrayIcon *tray, GUIState *gui)
{
    (void)gui; tray->visible = 1;
}
void tray_hide(TrayIcon *tray, GUIState *gui)
{
    (void)gui; tray->visible = 0;
}
void tray_set_tooltip(TrayIcon *tray, GUIState *gui, const char *text)
{
    (void)gui;
    snprintf(tray->tooltip, sizeof(tray->tooltip), "%s", text);
}
void tray_notify(TrayIcon *tray, GUIState *gui,
                 const char *title, const char *msg)
{
    (void)tray; (void)gui; (void)title; (void)msg;
}

#endif
