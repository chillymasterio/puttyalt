/*
 * puttyalt_dialogs.c: Dark-themed Win32 dialogs with in-memory templates.
 */

#include "puttyalt_dialogs.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* ── Command-line parser (cross-platform) ── */

int parse_connect_args(int argc, char **argv, ConnectParams *params)
{
    memset(params, 0, sizeof(*params));
    params->port = 22;
    params->protocol = 0; /* SSH */

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-P") == 0 && i + 1 < argc) {
            params->port = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-l") == 0 && i + 1 < argc) {
            snprintf(params->username, sizeof(params->username), "%s", argv[++i]);
        } else if (strcmp(argv[i], "-pw") == 0 && i + 1 < argc) {
            snprintf(params->password, sizeof(params->password), "%s", argv[++i]);
        } else if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
            snprintf(params->keyfile, sizeof(params->keyfile), "%s", argv[++i]);
        } else if (strcmp(argv[i], "-telnet") == 0) {
            params->protocol = 1;
            if (params->port == 22) params->port = 23;
        } else if (strcmp(argv[i], "-serial") == 0) {
            params->protocol = 2;
        } else if (strcmp(argv[i], "-raw") == 0) {
            params->protocol = 3;
        } else if (strcmp(argv[i], "-load") == 0 && i + 1 < argc) {
            snprintf(params->session_name, sizeof(params->session_name), "%s", argv[++i]);
        } else if (argv[i][0] != '-') {
            /* Parse user@host:port */
            char *at = strchr(argv[i], '@');
            char *host_start = argv[i];

            if (at) {
                *at = '\0';
                snprintf(params->username, sizeof(params->username), "%s", argv[i]);
                host_start = at + 1;
            }

            char *colon = strchr(host_start, ':');
            if (colon) {
                *colon = '\0';
                params->port = atoi(colon + 1);
            }

            snprintf(params->hostname, sizeof(params->hostname), "%s", host_start);
        }
    }

    return params->hostname[0] ? 0 : -1;
}

#ifdef _WIN32
#include <windows.h>
#include <commctrl.h>
#include <commdlg.h>

/* ── Color helpers ── */
#define HEX_R(c) (((c)>>16)&0xFF)
#define HEX_G(c) (((c)>>8)&0xFF)
#define HEX_B(c) ((c)&0xFF)
#define HEX_RGB(c) RGB(HEX_R(c),HEX_G(c),HEX_B(c))

#define IDC_FIND_TEXT 1301

/* ── Dark theme brushes ── */
static HBRUSH s_bg = NULL;
static HBRUSH s_ed = NULL;

static void init_brushes(void)
{
    if (!s_bg) {
        s_bg = CreateSolidBrush(HEX_RGB(GUI_COLOR_BG));
        s_ed = CreateSolidBrush(HEX_RGB(GUI_COLOR_SURFACE));
    }
}

/* ── In-memory dialog template builder ── */

typedef struct {
    union { DWORD _align; BYTE buf[4096]; };
    int pos;
} DlgBuf;

static void db_a4(DlgBuf *d) { d->pos = (d->pos + 3) & ~3; }

static void db_w(DlgBuf *d, WORD v)
{
    memcpy(d->buf + d->pos, &v, 2);
    d->pos += 2;
}

static void db_dw(DlgBuf *d, DWORD v)
{
    memcpy(d->buf + d->pos, &v, 4);
    d->pos += 4;
}

static void db_s(DlgBuf *d, short v)
{
    memcpy(d->buf + d->pos, &v, 2);
    d->pos += 2;
}

static void db_ws(DlgBuf *d, const char *s)
{
    while (*s) {
        WCHAR w = (WCHAR)(unsigned char)*s++;
        memcpy(d->buf + d->pos, &w, 2);
        d->pos += 2;
    }
    WCHAR z = 0;
    memcpy(d->buf + d->pos, &z, 2);
    d->pos += 2;
}

static void db_hdr(DlgBuf *d, int n, int cx, int cy, const char *title)
{
    d->pos = 0;
    db_dw(d, DS_SETFONT | DS_MODALFRAME | DS_CENTER |
             WS_POPUP | WS_CAPTION | WS_SYSMENU);
    db_dw(d, 0);
    db_w(d, (WORD)n);
    db_s(d, 0); db_s(d, 0);
    db_s(d, (short)cx); db_s(d, (short)cy);
    db_w(d, 0); db_w(d, 0);
    db_ws(d, title);
    db_w(d, 9);
    db_ws(d, "Segoe UI");
}

#define CLS_BTN  0x0080
#define CLS_EDT  0x0081
#define CLS_LBL  0x0082
#define CLS_LST  0x0083
#define CLS_CMB  0x0085

static void db_ctl(DlgBuf *d, DWORD sty, short x, short y, short cx, short cy,
                   WORD id, WORD cls, const char *text)
{
    db_a4(d);
    db_dw(d, sty | WS_CHILD | WS_VISIBLE);
    db_dw(d, 0);
    db_s(d, x); db_s(d, y); db_s(d, cx); db_s(d, cy);
    db_w(d, id);
    db_w(d, 0xFFFF); db_w(d, cls);
    db_ws(d, text);
    db_w(d, 0);
}

/* ── Dark theme color handler ── */

static INT_PTR dark_color(UINT msg, WPARAM wp)
{
    HDC hdc = (HDC)wp;
    switch (msg) {
    case WM_CTLCOLORDLG:
        return (INT_PTR)s_bg;
    case WM_CTLCOLORSTATIC:
        SetTextColor(hdc, HEX_RGB(GUI_COLOR_TEXT));
        SetBkColor(hdc, HEX_RGB(GUI_COLOR_BG));
        return (INT_PTR)s_bg;
    case WM_CTLCOLOREDIT:
        SetTextColor(hdc, HEX_RGB(GUI_COLOR_TEXT));
        SetBkColor(hdc, HEX_RGB(GUI_COLOR_SURFACE));
        return (INT_PTR)s_ed;
    case WM_CTLCOLORLISTBOX:
        SetTextColor(hdc, HEX_RGB(GUI_COLOR_TEXT));
        SetBkColor(hdc, HEX_RGB(GUI_COLOR_SURFACE));
        return (INT_PTR)s_ed;
    }
    return 0;
}

/* ── Owner-drawn button painting ── */

static void paint_btn(DRAWITEMSTRUCT *di, int primary)
{
    HBRUSH br;
    COLORREF tc;

    if (di->itemState & ODS_SELECTED) {
        br = CreateSolidBrush(HEX_RGB(0x3580C0));
        tc = HEX_RGB(0xFFFFFF);
    } else if (primary) {
        br = CreateSolidBrush(HEX_RGB(GUI_COLOR_PRIMARY));
        tc = HEX_RGB(0xFFFFFF);
    } else {
        br = CreateSolidBrush(HEX_RGB(GUI_COLOR_SURFACE));
        tc = HEX_RGB(GUI_COLOR_TEXT);
    }

    FillRect(di->hDC, &di->rcItem, br);
    DeleteObject(br);

    /* Border */
    HPEN pen = CreatePen(PS_SOLID, 1, HEX_RGB(GUI_COLOR_BORDER));
    HPEN old = (HPEN)SelectObject(di->hDC, pen);
    SelectObject(di->hDC, GetStockObject(NULL_BRUSH));
    Rectangle(di->hDC, di->rcItem.left, di->rcItem.top,
              di->rcItem.right, di->rcItem.bottom);
    SelectObject(di->hDC, old);
    DeleteObject(pen);

    /* Text */
    SetBkMode(di->hDC, TRANSPARENT);
    SetTextColor(di->hDC, tc);
    char txt[64];
    GetWindowTextA(di->hwndItem, txt, sizeof(txt));
    DrawTextA(di->hDC, txt, -1, &di->rcItem,
              DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    if (di->itemState & ODS_FOCUS) {
        RECT fr = di->rcItem;
        InflateRect(&fr, -3, -3);
        DrawFocusRect(di->hDC, &fr);
    }
}

/* ════════════════════════════════════════════════
 *  Connect Dialog
 * ════════════════════════════════════════════════ */

static INT_PTR CALLBACK connect_proc(HWND hwnd, UINT msg,
                                      WPARAM wp, LPARAM lp)
{
    ConnectParams *p;

    switch (msg) {
    case WM_INITDIALOG:
        SetWindowLongPtrA(hwnd, GWLP_USERDATA, lp);
        p = (ConnectParams *)lp;

        if (p->hostname[0])
            SetDlgItemTextA(hwnd, IDC_HOST, p->hostname);
        SetDlgItemInt(hwnd, IDC_PORT, p->port ? p->port : 22, FALSE);
        if (p->username[0])
            SetDlgItemTextA(hwnd, IDC_USER, p->username);

        SendDlgItemMessage(hwnd, IDC_PROTOCOL, CB_ADDSTRING, 0, (LPARAM)"SSH");
        SendDlgItemMessage(hwnd, IDC_PROTOCOL, CB_ADDSTRING, 0, (LPARAM)"Telnet");
        SendDlgItemMessage(hwnd, IDC_PROTOCOL, CB_ADDSTRING, 0, (LPARAM)"Serial");
        SendDlgItemMessage(hwnd, IDC_PROTOCOL, CB_ADDSTRING, 0, (LPARAM)"Raw");
        SendDlgItemMessage(hwnd, IDC_PROTOCOL, CB_SETCURSEL, p->protocol, 0);

        SendMessage(hwnd, DM_SETDEFID, IDC_CONNECT, 0);
        SetFocus(GetDlgItem(hwnd, IDC_HOST));
        return FALSE;

    case WM_CTLCOLORDLG:
    case WM_CTLCOLORSTATIC:
    case WM_CTLCOLOREDIT:
    case WM_CTLCOLORLISTBOX:
        return dark_color(msg, wp);

    case WM_DRAWITEM:
        if (((DRAWITEMSTRUCT *)lp)->CtlType == ODT_BUTTON) {
            paint_btn((DRAWITEMSTRUCT *)lp,
                      ((DRAWITEMSTRUCT *)lp)->CtlID == IDC_CONNECT);
            return TRUE;
        }
        break;

    case WM_COMMAND:
        p = (ConnectParams *)GetWindowLongPtrA(hwnd, GWLP_USERDATA);

        switch (LOWORD(wp)) {
        case IDC_CONNECT:
        case IDOK:
            GetDlgItemTextA(hwnd, IDC_HOST, p->hostname,
                            sizeof(p->hostname));
            p->port = GetDlgItemInt(hwnd, IDC_PORT, NULL, FALSE);
            GetDlgItemTextA(hwnd, IDC_USER, p->username,
                            sizeof(p->username));
            GetDlgItemTextA(hwnd, IDC_PASS, p->password,
                            sizeof(p->password));
            GetDlgItemTextA(hwnd, IDC_KEYFILE, p->keyfile,
                            sizeof(p->keyfile));
            p->protocol = (int)SendDlgItemMessage(hwnd, IDC_PROTOCOL,
                                                   CB_GETCURSEL, 0, 0);
            if (!p->hostname[0]) {
                MessageBoxA(hwnd, "Please enter a hostname.",
                            PUTTYALT_NAME, MB_OK | MB_ICONWARNING);
                return TRUE;
            }
            if (!p->port) p->port = 22;
            EndDialog(hwnd, IDOK);
            return TRUE;

        case IDCANCEL:
            EndDialog(hwnd, IDCANCEL);
            return TRUE;

        case IDC_BROWSE: {
            OPENFILENAMEA ofn;
            char path[512] = "";
            memset(&ofn, 0, sizeof(ofn));
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = hwnd;
            ofn.lpstrFilter = "PuTTY Keys (*.ppk)\0*.ppk\0"
                              "OpenSSH Keys (*)\0*\0"
                              "All Files\0*.*\0";
            ofn.lpstrFile = path;
            ofn.nMaxFile = sizeof(path);
            ofn.Flags = OFN_FILEMUSTEXIST;
            if (GetOpenFileNameA(&ofn))
                SetDlgItemTextA(hwnd, IDC_KEYFILE, path);
            return TRUE;
        }

        case IDC_PROTOCOL:
            if (HIWORD(wp) == CBN_SELCHANGE) {
                int proto = (int)SendDlgItemMessage(hwnd, IDC_PROTOCOL,
                                                     CB_GETCURSEL, 0, 0);
                int dp = (proto == 0) ? 22 : (proto == 1) ? 23 : 0;
                if (dp) SetDlgItemInt(hwnd, IDC_PORT, dp, FALSE);
            }
            return TRUE;
        }
        break;

    case WM_CLOSE:
        EndDialog(hwnd, IDCANCEL);
        return TRUE;
    }
    return FALSE;
}

int dialog_connect(GUIState *gui, ConnectParams *params)
{
    init_brushes();

    DlgBuf d;
    db_hdr(&d, 15, 260, 160, "Connect to Server");

    /* Row 1: Host + Port */
    db_ctl(&d, SS_LEFT, 10, 12, 44, 8,
           (WORD)-1, CLS_LBL, "Host / IP:");
    db_ctl(&d, ES_AUTOHSCROLL | WS_BORDER | WS_TABSTOP, 58, 10, 130, 14,
           IDC_HOST, CLS_EDT, "");
    db_ctl(&d, SS_LEFT, 195, 12, 20, 8,
           (WORD)-1, CLS_LBL, "Port:");
    db_ctl(&d, ES_NUMBER | WS_BORDER | WS_TABSTOP, 220, 10, 30, 14,
           IDC_PORT, CLS_EDT, "");

    /* Row 2: Username */
    db_ctl(&d, SS_LEFT, 10, 32, 44, 8,
           (WORD)-1, CLS_LBL, "Username:");
    db_ctl(&d, ES_AUTOHSCROLL | WS_BORDER | WS_TABSTOP, 58, 30, 192, 14,
           IDC_USER, CLS_EDT, "");

    /* Row 3: Password */
    db_ctl(&d, SS_LEFT, 10, 52, 44, 8,
           (WORD)-1, CLS_LBL, "Password:");
    db_ctl(&d, ES_PASSWORD | ES_AUTOHSCROLL | WS_BORDER | WS_TABSTOP,
           58, 50, 192, 14, IDC_PASS, CLS_EDT, "");

    /* Row 4: Protocol */
    db_ctl(&d, SS_LEFT, 10, 72, 44, 8,
           (WORD)-1, CLS_LBL, "Protocol:");
    db_ctl(&d, CBS_DROPDOWNLIST | WS_TABSTOP, 58, 69, 80, 100,
           IDC_PROTOCOL, CLS_CMB, "");

    /* Row 5: Key file */
    db_ctl(&d, SS_LEFT, 10, 92, 44, 8,
           (WORD)-1, CLS_LBL, "Key File:");
    db_ctl(&d, ES_AUTOHSCROLL | WS_BORDER | WS_TABSTOP, 58, 90, 140, 14,
           IDC_KEYFILE, CLS_EDT, "");
    db_ctl(&d, BS_OWNERDRAW | WS_TABSTOP, 204, 89, 46, 14,
           IDC_BROWSE, CLS_BTN, "Browse...");

    /* Buttons */
    db_ctl(&d, BS_OWNERDRAW | WS_TABSTOP, 142, 138, 55, 16,
           IDC_CONNECT, CLS_BTN, "Connect");
    db_ctl(&d, BS_OWNERDRAW | WS_TABSTOP, 202, 138, 48, 16,
           IDCANCEL, CLS_BTN, "Cancel");

    INT_PTR ret = DialogBoxIndirectParamA(
        (HINSTANCE)gui->hinstance,
        (DLGTEMPLATE *)d.buf,
        (HWND)gui->hwnd,
        connect_proc,
        (LPARAM)params);

    return (ret == IDOK) ? 0 : -1;
}

/* ════════════════════════════════════════════════
 *  Settings Dialog
 * ════════════════════════════════════════════════ */

static INT_PTR CALLBACK settings_proc(HWND hwnd, UINT msg,
                                       WPARAM wp, LPARAM lp)
{
    GUIState *gui;

    switch (msg) {
    case WM_INITDIALOG:
        SetWindowLongPtrA(hwnd, GWLP_USERDATA, lp);
        gui = (GUIState *)lp;
        SetDlgItemTextA(hwnd, IDC_FONT_NAME, gui->config.font_name);
        SetDlgItemInt(hwnd, IDC_FONT_SIZE, gui->config.font_size, FALSE);
        SetDlgItemInt(hwnd, IDC_SCROLLBACK,
                      gui->config.scrollback_lines, FALSE);
        SetDlgItemInt(hwnd, IDC_TRANSPARENCY,
                      gui->config.transparency, FALSE);
        CheckDlgButton(hwnd, IDC_BELL_CHECK,
                       gui->config.bell_enabled ? BST_CHECKED : BST_UNCHECKED);
        CheckDlgButton(hwnd, IDC_CONFIRM_CHK,
                       gui->config.confirm_on_close ? BST_CHECKED : BST_UNCHECKED);
        return TRUE;

    case WM_CTLCOLORDLG:
    case WM_CTLCOLORSTATIC:
    case WM_CTLCOLOREDIT:
        return dark_color(msg, wp);

    case WM_DRAWITEM:
        if (((DRAWITEMSTRUCT *)lp)->CtlType == ODT_BUTTON) {
            paint_btn((DRAWITEMSTRUCT *)lp,
                      ((DRAWITEMSTRUCT *)lp)->CtlID == IDOK);
            return TRUE;
        }
        break;

    case WM_COMMAND:
        gui = (GUIState *)GetWindowLongPtrA(hwnd, GWLP_USERDATA);

        switch (LOWORD(wp)) {
        case IDOK:
            GetDlgItemTextA(hwnd, IDC_FONT_NAME,
                            gui->config.font_name,
                            sizeof(gui->config.font_name));
            gui->config.font_size =
                GetDlgItemInt(hwnd, IDC_FONT_SIZE, NULL, FALSE);
            gui->config.scrollback_lines =
                GetDlgItemInt(hwnd, IDC_SCROLLBACK, NULL, FALSE);
            gui->config.transparency =
                GetDlgItemInt(hwnd, IDC_TRANSPARENCY, NULL, FALSE);
            gui->config.bell_enabled =
                IsDlgButtonChecked(hwnd, IDC_BELL_CHECK) == BST_CHECKED;
            gui->config.confirm_on_close =
                IsDlgButtonChecked(hwnd, IDC_CONFIRM_CHK) == BST_CHECKED;
            gui_terminal_set_font(gui, gui->config.font_name,
                                  gui->config.font_size);
            EndDialog(hwnd, IDOK);
            return TRUE;

        case IDCANCEL:
            EndDialog(hwnd, IDCANCEL);
            return TRUE;
        }
        break;

    case WM_CLOSE:
        EndDialog(hwnd, IDCANCEL);
        return TRUE;
    }
    return FALSE;
}

int dialog_settings(GUIState *gui)
{
    init_brushes();

    DlgBuf d;
    db_hdr(&d, 12, 240, 190, "Preferences");

    db_ctl(&d, SS_LEFT, 10, 12, 52, 8,
           (WORD)-1, CLS_LBL, "Font Name:");
    db_ctl(&d, ES_AUTOHSCROLL | WS_BORDER | WS_TABSTOP, 68, 10, 162, 14,
           IDC_FONT_NAME, CLS_EDT, "");

    db_ctl(&d, SS_LEFT, 10, 32, 52, 8,
           (WORD)-1, CLS_LBL, "Font Size:");
    db_ctl(&d, ES_NUMBER | WS_BORDER | WS_TABSTOP, 68, 30, 40, 14,
           IDC_FONT_SIZE, CLS_EDT, "");

    db_ctl(&d, SS_LEFT, 10, 52, 52, 8,
           (WORD)-1, CLS_LBL, "Scrollback:");
    db_ctl(&d, ES_NUMBER | WS_BORDER | WS_TABSTOP, 68, 50, 60, 14,
           IDC_SCROLLBACK, CLS_EDT, "");

    db_ctl(&d, SS_LEFT, 10, 72, 55, 8,
           (WORD)-1, CLS_LBL, "Transparency:");
    db_ctl(&d, ES_NUMBER | WS_BORDER | WS_TABSTOP, 72, 70, 40, 14,
           IDC_TRANSPARENCY, CLS_EDT, "");

    db_ctl(&d, BS_AUTOCHECKBOX | WS_TABSTOP, 10, 96, 100, 12,
           IDC_BELL_CHECK, CLS_BTN, "Enable bell");
    db_ctl(&d, BS_AUTOCHECKBOX | WS_TABSTOP, 10, 114, 120, 12,
           IDC_CONFIRM_CHK, CLS_BTN, "Confirm on close");

    db_ctl(&d, BS_OWNERDRAW | WS_TABSTOP, 120, 168, 55, 16,
           IDOK, CLS_BTN, "Save");
    db_ctl(&d, BS_OWNERDRAW | WS_TABSTOP, 180, 168, 50, 16,
           IDCANCEL, CLS_BTN, "Cancel");

    return DialogBoxIndirectParamA(
        (HINSTANCE)gui->hinstance,
        (DLGTEMPLATE *)d.buf,
        (HWND)gui->hwnd,
        settings_proc,
        (LPARAM)gui) == IDOK ? 0 : -1;
}

/* ════════════════════════════════════════════════
 *  Find Dialog
 * ════════════════════════════════════════════════ */

static INT_PTR CALLBACK find_proc(HWND hwnd, UINT msg,
                                   WPARAM wp, LPARAM lp)
{
    switch (msg) {
    case WM_INITDIALOG:
        SetWindowLongPtrA(hwnd, GWLP_USERDATA, lp);
        SetFocus(GetDlgItem(hwnd, IDC_FIND_TEXT));
        return FALSE;

    case WM_CTLCOLORDLG:
    case WM_CTLCOLORSTATIC:
    case WM_CTLCOLOREDIT:
        return dark_color(msg, wp);

    case WM_DRAWITEM:
        if (((DRAWITEMSTRUCT *)lp)->CtlType == ODT_BUTTON) {
            paint_btn((DRAWITEMSTRUCT *)lp,
                      ((DRAWITEMSTRUCT *)lp)->CtlID == IDOK);
            return TRUE;
        }
        break;

    case WM_COMMAND:
        switch (LOWORD(wp)) {
        case IDOK: {
            char *buf = (char *)GetWindowLongPtrA(hwnd, GWLP_USERDATA);
            GetDlgItemTextA(hwnd, IDC_FIND_TEXT, buf, 256);
            EndDialog(hwnd, IDOK);
            return TRUE;
        }
        case IDCANCEL:
            EndDialog(hwnd, IDCANCEL);
            return TRUE;
        }
        break;

    case WM_CLOSE:
        EndDialog(hwnd, IDCANCEL);
        return TRUE;
    }
    return FALSE;
}

int dialog_find(GUIState *gui, char *search_text, int max_len)
{
    init_brushes();

    DlgBuf d;
    db_hdr(&d, 4, 250, 55, "Find in Terminal");

    db_ctl(&d, SS_LEFT, 10, 14, 22, 8,
           (WORD)-1, CLS_LBL, "Find:");
    db_ctl(&d, ES_AUTOHSCROLL | WS_BORDER | WS_TABSTOP,
           36, 12, 138, 14, IDC_FIND_TEXT, CLS_EDT, "");
    db_ctl(&d, BS_OWNERDRAW | WS_TABSTOP,
           180, 11, 58, 14, IDOK, CLS_BTN, "Find Next");
    db_ctl(&d, BS_OWNERDRAW | WS_TABSTOP,
           180, 32, 58, 14, IDCANCEL, CLS_BTN, "Close");

    memset(search_text, 0, max_len);

    return DialogBoxIndirectParamA(
        (HINSTANCE)gui->hinstance,
        (DLGTEMPLATE *)d.buf,
        (HWND)gui->hwnd,
        find_proc,
        (LPARAM)search_text) == IDOK ? 0 : -1;
}

/* ════════════════════════════════════════════════
 *  Theme Selection Dialog
 * ════════════════════════════════════════════════ */

static INT_PTR CALLBACK theme_proc(HWND hwnd, UINT msg,
                                    WPARAM wp, LPARAM lp)
{
    switch (msg) {
    case WM_INITDIALOG:
        SetWindowLongPtrA(hwnd, GWLP_USERDATA, lp);
        SendDlgItemMessage(hwnd, IDC_THEME_LIST, LB_ADDSTRING, 0,
                           (LPARAM)"Warm Blue (Default)");
        SendDlgItemMessage(hwnd, IDC_THEME_LIST, LB_ADDSTRING, 0,
                           (LPARAM)"Dark Midnight");
        SendDlgItemMessage(hwnd, IDC_THEME_LIST, LB_ADDSTRING, 0,
                           (LPARAM)"Ocean Deep");
        SendDlgItemMessage(hwnd, IDC_THEME_LIST, LB_ADDSTRING, 0,
                           (LPARAM)"Slate Gray");
        SendDlgItemMessage(hwnd, IDC_THEME_LIST, LB_ADDSTRING, 0,
                           (LPARAM)"Forest Night");
        SendDlgItemMessage(hwnd, IDC_THEME_LIST, LB_SETCURSEL, 0, 0);
        return TRUE;

    case WM_CTLCOLORDLG:
    case WM_CTLCOLORSTATIC:
    case WM_CTLCOLORLISTBOX:
        return dark_color(msg, wp);

    case WM_DRAWITEM:
        if (((DRAWITEMSTRUCT *)lp)->CtlType == ODT_BUTTON) {
            paint_btn((DRAWITEMSTRUCT *)lp,
                      ((DRAWITEMSTRUCT *)lp)->CtlID == IDOK);
            return TRUE;
        }
        break;

    case WM_COMMAND:
        switch (LOWORD(wp)) {
        case IDOK: {
            GUIState *gui = (GUIState *)GetWindowLongPtrA(hwnd,
                                                           GWLP_USERDATA);
            int sel = (int)SendDlgItemMessage(hwnd, IDC_THEME_LIST,
                                               LB_GETCURSEL, 0, 0);
            if (sel >= 0) {
                char name[64];
                SendDlgItemMessage(hwnd, IDC_THEME_LIST, LB_GETTEXT,
                                   sel, (LPARAM)name);
                snprintf(gui->config.theme_name,
                         sizeof(gui->config.theme_name), "%s", name);
            }
            EndDialog(hwnd, IDOK);
            return TRUE;
        }
        case IDCANCEL:
            EndDialog(hwnd, IDCANCEL);
            return TRUE;
        }
        break;

    case WM_CLOSE:
        EndDialog(hwnd, IDCANCEL);
        return TRUE;
    }
    return FALSE;
}

int dialog_theme_select(GUIState *gui)
{
    init_brushes();

    DlgBuf d;
    db_hdr(&d, 4, 200, 165, "Select Theme");

    db_ctl(&d, SS_LEFT, 10, 8, 80, 8,
           (WORD)-1, CLS_LBL, "Available themes:");
    db_ctl(&d, LBS_NOTIFY | WS_BORDER | WS_VSCROLL | WS_TABSTOP,
           10, 20, 180, 105, IDC_THEME_LIST, CLS_LST, "");
    db_ctl(&d, BS_OWNERDRAW | WS_TABSTOP,
           80, 142, 50, 16, IDOK, CLS_BTN, "Apply");
    db_ctl(&d, BS_OWNERDRAW | WS_TABSTOP,
           136, 142, 50, 16, IDCANCEL, CLS_BTN, "Cancel");

    return DialogBoxIndirectParamA(
        (HINSTANCE)gui->hinstance,
        (DLGTEMPLATE *)d.buf,
        (HWND)gui->hwnd,
        theme_proc,
        (LPARAM)gui) == IDOK ? 0 : -1;
}

/* ════════════════════════════════════════════════
 *  Info / Placeholder Dialogs
 * ════════════════════════════════════════════════ */

int dialog_snippet_manager(GUIState *gui)
{
    MessageBoxA((HWND)gui->hwnd,
        "Snippet Manager\n\n"
        "Organize and quick-insert frequently used\n"
        "commands and text templates.\n\n"
        "Coming in the next update.",
        PUTTYALT_NAME " - Snippets", MB_OK | MB_ICONINFORMATION);
    return 0;
}

int dialog_keygen(GUIState *gui)
{
    MessageBoxA((HWND)gui->hwnd,
        "SSH Key Generator\n\n"
        "Generate SSH key pairs:\n"
        "  - RSA (2048 / 4096 bit)\n"
        "  - Ed25519\n"
        "  - ECDSA\n\n"
        "Coming in the next update.",
        PUTTYALT_NAME " - Key Generator", MB_OK | MB_ICONINFORMATION);
    return 0;
}

int dialog_tunnel_manager(GUIState *gui)
{
    MessageBoxA((HWND)gui->hwnd,
        "Port Tunnel Manager\n\n"
        "Create and manage SSH tunnels:\n"
        "  - Local port forwarding\n"
        "  - Remote port forwarding\n"
        "  - Dynamic SOCKS proxy\n\n"
        "Coming in the next update.",
        PUTTYALT_NAME " - Tunnels", MB_OK | MB_ICONINFORMATION);
    return 0;
}

int dialog_bookmark_manager(GUIState *gui)
{
    MessageBoxA((HWND)gui->hwnd,
        "Bookmark Manager\n\n"
        "Organize your server bookmarks for\n"
        "quick access to frequent connections.\n\n"
        "Coming in the next update.",
        PUTTYALT_NAME " - Bookmarks", MB_OK | MB_ICONINFORMATION);
    return 0;
}

#else /* ── Unix stubs ── */

int dialog_connect(GUIState *gui, ConnectParams *params)
{
    (void)gui; (void)params; return -1;
}

int dialog_settings(GUIState *gui) { (void)gui; return -1; }
int dialog_snippet_manager(GUIState *gui) { (void)gui; return -1; }
int dialog_keygen(GUIState *gui) { (void)gui; return -1; }
int dialog_tunnel_manager(GUIState *gui) { (void)gui; return -1; }
int dialog_bookmark_manager(GUIState *gui) { (void)gui; return -1; }
int dialog_theme_select(GUIState *gui) { (void)gui; return -1; }

int dialog_find(GUIState *gui, char *search_text, int max_len)
{
    (void)gui; (void)search_text; (void)max_len; return -1;
}

#endif
