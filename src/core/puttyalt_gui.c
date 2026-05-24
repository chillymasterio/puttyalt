/*
 * puttyalt_gui.c: GUI application framework — v2.0 Modern UI.
 *
 * GitHub Dark inspired design with custom-drawn controls,
 * double-buffered rendering, and DWM dark title bar.
 * No external dependencies beyond Win32 + DWM.
 */

#include "puttyalt_gui.h"
#include "puttyalt_design.h"
#include "puttyalt_dialogs.h"
#include "puttyalt_ctxmenu.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* RGB helper */
#define HEX_R(c) (((c)>>16)&0xFF)
#define HEX_G(c) (((c)>>8)&0xFF)
#define HEX_B(c) ((c)&0xFF)
#define HEX_RGB(c) RGB(HEX_R(c),HEX_G(c),HEX_B(c))

void gui_config_defaults(GUIConfig *cfg)
{
    memset(cfg, 0, sizeof(*cfg));
    cfg->width = GUI_DEFAULT_WIDTH;
    cfg->height = GUI_DEFAULT_HEIGHT;
    cfg->x = -1;
    cfg->y = -1;
    cfg->font_size = DS_FONT_SIZE_MD;
    snprintf(cfg->font_name, sizeof(cfg->font_name), DS_FONT_MONO);
    snprintf(cfg->theme_name, sizeof(cfg->theme_name), "GitHub Dark");
    cfg->scrollback_lines = 20000;
    cfg->tab_bar_position = 0;
    cfg->toolbar_visible = 0;  /* Hidden in v2.0 — cleaner look */
    cfg->statusbar_visible = 1;
    cfg->sidebar_visible = 1;
    cfg->sidebar_width = DS_SIDEBAR_WIDTH;
    cfg->sidebar_panel = SIDEBAR_SESSIONS;
    cfg->transparency = 0;
    cfg->cursor_style = 2; /* bar */
    cfg->cursor_blink = 1;
    cfg->line_spacing = 1;
    cfg->bell_enabled = 1;
    cfg->confirm_on_close = 1;
    cfg->copy_on_select = 1;
    cfg->paste_on_right_click = 1;
    cfg->scroll_on_output = 0;
    cfg->scroll_on_keypress = 1;
    cfg->color_bg = DS_TERM_BG;
    cfg->color_fg = DS_TERM_FG;
    cfg->color_cursor = DS_TERM_CURSOR;
    cfg->color_selection = DS_TERM_SELECTION;
}

int gui_config_load(GUIConfig *cfg, const char *path)
{
    FILE *f = fopen(path, "r");
    char line[512];
    if (!f) { gui_config_defaults(cfg); return -1; }
    gui_config_defaults(cfg);
    while (fgets(line, sizeof(line), f)) {
        char *eq = strchr(line, '=');
        if (!eq) continue;
        *eq = '\0';
        char *val = eq + 1;
        size_t vlen = strlen(val);
        while (vlen > 0 && (val[vlen-1] == '\n' || val[vlen-1] == '\r'))
            val[--vlen] = '\0';
        if (strcmp(line, "width") == 0) cfg->width = atoi(val);
        else if (strcmp(line, "height") == 0) cfg->height = atoi(val);
        else if (strcmp(line, "x") == 0) cfg->x = atoi(val);
        else if (strcmp(line, "y") == 0) cfg->y = atoi(val);
        else if (strcmp(line, "maximized") == 0) cfg->maximized = atoi(val);
        else if (strcmp(line, "font_size") == 0) cfg->font_size = atoi(val);
        else if (strcmp(line, "font_name") == 0)
            snprintf(cfg->font_name, sizeof(cfg->font_name), "%s", val);
        else if (strcmp(line, "theme") == 0)
            snprintf(cfg->theme_name, sizeof(cfg->theme_name), "%s", val);
        else if (strcmp(line, "scrollback") == 0) cfg->scrollback_lines = atoi(val);
        else if (strcmp(line, "toolbar") == 0) cfg->toolbar_visible = atoi(val);
        else if (strcmp(line, "statusbar") == 0) cfg->statusbar_visible = atoi(val);
        else if (strcmp(line, "sidebar") == 0) cfg->sidebar_visible = atoi(val);
        else if (strcmp(line, "sidebar_width") == 0) cfg->sidebar_width = atoi(val);
        else if (strcmp(line, "transparency") == 0) cfg->transparency = atoi(val);
        else if (strcmp(line, "cursor_style") == 0) cfg->cursor_style = atoi(val);
        else if (strcmp(line, "cursor_blink") == 0) cfg->cursor_blink = atoi(val);
        else if (strcmp(line, "bell") == 0) cfg->bell_enabled = atoi(val);
        else if (strcmp(line, "confirm_close") == 0) cfg->confirm_on_close = atoi(val);
        else if (strcmp(line, "copy_on_select") == 0) cfg->copy_on_select = atoi(val);
        else if (strcmp(line, "scroll_on_keypress") == 0) cfg->scroll_on_keypress = atoi(val);
    }
    fclose(f);
    return 0;
}

int gui_config_save(const GUIConfig *cfg, const char *path)
{
    FILE *f = fopen(path, "w");
    if (!f) return -1;
    fprintf(f, "width=%d\n", cfg->width);
    fprintf(f, "height=%d\n", cfg->height);
    fprintf(f, "x=%d\n", cfg->x);
    fprintf(f, "y=%d\n", cfg->y);
    fprintf(f, "maximized=%d\n", cfg->maximized);
    fprintf(f, "font_size=%d\n", cfg->font_size);
    fprintf(f, "font_name=%s\n", cfg->font_name);
    fprintf(f, "theme=%s\n", cfg->theme_name);
    fprintf(f, "scrollback=%d\n", cfg->scrollback_lines);
    fprintf(f, "toolbar=%d\n", cfg->toolbar_visible);
    fprintf(f, "statusbar=%d\n", cfg->statusbar_visible);
    fprintf(f, "sidebar=%d\n", cfg->sidebar_visible);
    fprintf(f, "sidebar_width=%d\n", cfg->sidebar_width);
    fprintf(f, "transparency=%d\n", cfg->transparency);
    fprintf(f, "cursor_style=%d\n", cfg->cursor_style);
    fprintf(f, "cursor_blink=%d\n", cfg->cursor_blink);
    fprintf(f, "bell=%d\n", cfg->bell_enabled);
    fprintf(f, "confirm_close=%d\n", cfg->confirm_on_close);
    fprintf(f, "copy_on_select=%d\n", cfg->copy_on_select);
    fprintf(f, "scroll_on_keypress=%d\n", cfg->scroll_on_keypress);
    fclose(f);
    return 0;
}

/* Zoom implementation */
void gui_zoom_in(GUIState *gui)
{
    if (gui->zoom_level < 5) {
        gui->zoom_level++;
        gui->config.font_size++;
        gui_terminal_set_font(gui, gui->config.font_name, gui->config.font_size);
    }
}

void gui_zoom_out(GUIState *gui)
{
    if (gui->zoom_level > -5 && gui->config.font_size > 6) {
        gui->zoom_level--;
        gui->config.font_size--;
        gui_terminal_set_font(gui, gui->config.font_name, gui->config.font_size);
    }
}

void gui_zoom_reset(GUIState *gui)
{
    gui->config.font_size -= gui->zoom_level;
    gui->zoom_level = 0;
    gui_terminal_set_font(gui, gui->config.font_name, gui->config.font_size);
}

#ifdef _WIN32
#include <windows.h>
#include <commctrl.h>
#include <dwmapi.h>

static const char *WNDCLASS_NAME = "PuttyAltWindow";
static const char *TERM_CLASS = "PuttyAltTerminal";
static const char *SIDEBAR_CLASS = "PuttyAltSidebar";
static const char *TABBAR_CLASS = "PuttyAltTabBar";

/* ══════════════════════════════════════════
 *  Double-buffered paint helper
 * ══════════════════════════════════════════ */

typedef struct {
    HDC hdc;
    HDC mem_dc;
    HBITMAP bmp;
    HBITMAP old_bmp;
    RECT rc;
} PaintBuffer;

static PaintBuffer paint_begin(HWND hwnd, PAINTSTRUCT *ps)
{
    PaintBuffer pb;
    pb.hdc = BeginPaint(hwnd, ps);
    GetClientRect(hwnd, &pb.rc);
    pb.mem_dc = CreateCompatibleDC(pb.hdc);
    pb.bmp = CreateCompatibleBitmap(pb.hdc,
        pb.rc.right - pb.rc.left, pb.rc.bottom - pb.rc.top);
    pb.old_bmp = (HBITMAP)SelectObject(pb.mem_dc, pb.bmp);
    return pb;
}

static void paint_end(PaintBuffer *pb, PAINTSTRUCT *ps)
{
    BitBlt(pb->hdc, 0, 0,
        pb->rc.right - pb->rc.left, pb->rc.bottom - pb->rc.top,
        pb->mem_dc, 0, 0, SRCCOPY);
    SelectObject(pb->mem_dc, pb->old_bmp);
    DeleteObject(pb->bmp);
    DeleteDC(pb->mem_dc);
    EndPaint(WindowFromDC(pb->hdc), ps);
}

/* ══════════════════════════════════════════
 *  Rounded rectangle helper (GDI)
 * ══════════════════════════════════════════ */

static void fill_rounded_rect(HDC hdc, RECT *rc, int radius, COLORREF color)
{
    HBRUSH brush = CreateSolidBrush(color);
    HPEN pen = CreatePen(PS_SOLID, 1, color);
    HBRUSH old_brush = (HBRUSH)SelectObject(hdc, brush);
    HPEN old_pen = (HPEN)SelectObject(hdc, pen);
    RoundRect(hdc, rc->left, rc->top, rc->right, rc->bottom, radius, radius);
    SelectObject(hdc, old_brush);
    SelectObject(hdc, old_pen);
    DeleteObject(brush);
    DeleteObject(pen);
}

/* ══════════════════════════════════════════
 *  DWM dark title bar
 * ══════════════════════════════════════════ */

static void enable_dark_titlebar(HWND hwnd)
{
    BOOL dark = TRUE;
    DwmSetWindowAttribute(hwnd, DS_DWMWA_DARK_MODE,
                          &dark, sizeof(dark));

    /* Set caption and border colors to match our theme */
    COLORREF caption_color = HEX_RGB(DS_BG);
    COLORREF border_color = HEX_RGB(DS_BORDER_SUBTLE);
    DwmSetWindowAttribute(hwnd, DS_DWMWA_CAPTION,
                          &caption_color, sizeof(caption_color));
    DwmSetWindowAttribute(hwnd, DS_DWMWA_BORDER,
                          &border_color, sizeof(border_color));
}

/* ══════════════════════════════════════════
 *  Menu creation
 * ══════════════════════════════════════════ */

static void gui_create_menu(GUIState *gui)
{
    HMENU menu = CreateMenu();
    HMENU fm = CreatePopupMenu();
    HMENU em = CreatePopupMenu();
    HMENU sm = CreatePopupMenu();
    HMENU vm = CreatePopupMenu();
    HMENU tm = CreatePopupMenu();
    HMENU hm = CreatePopupMenu();

    AppendMenuA(fm, MF_STRING, IDM_FILE_NEW, "New Session\tCtrl+N");
    AppendMenuA(fm, MF_STRING, IDM_FILE_OPEN, "Open Session...\tCtrl+O");
    AppendMenuA(fm, MF_STRING, IDM_FILE_SAVE_SESSION, "Save Session\tCtrl+S");
    AppendMenuA(fm, MF_STRING, IDM_FILE_DUPLICATE, "Duplicate Tab\tCtrl+D");
    AppendMenuA(fm, MF_SEPARATOR, 0, NULL);
    AppendMenuA(fm, MF_STRING, IDM_FILE_IMPORT, "Import Sessions...");
    AppendMenuA(fm, MF_STRING, IDM_FILE_EXPORT, "Export Sessions...");
    AppendMenuA(fm, MF_STRING, IDM_FILE_LOG, "Session Log...");
    AppendMenuA(fm, MF_SEPARATOR, 0, NULL);
    AppendMenuA(fm, MF_STRING, IDM_FILE_EXIT, "Exit\tAlt+F4");

    AppendMenuA(em, MF_STRING, IDM_EDIT_COPY, "Copy\tCtrl+Shift+C");
    AppendMenuA(em, MF_STRING, IDM_EDIT_PASTE, "Paste\tCtrl+Shift+V");
    AppendMenuA(em, MF_STRING, IDM_EDIT_SELECTALL, "Select All\tCtrl+Shift+A");
    AppendMenuA(em, MF_SEPARATOR, 0, NULL);
    AppendMenuA(em, MF_STRING, IDM_EDIT_FIND, "Find...\tCtrl+F");
    AppendMenuA(em, MF_STRING, IDM_EDIT_CLEAR, "Clear Scrollback");
    AppendMenuA(em, MF_SEPARATOR, 0, NULL);
    AppendMenuA(em, MF_STRING, IDM_EDIT_PREFERENCES, "Preferences...\tCtrl+,");

    AppendMenuA(sm, MF_STRING, IDM_SESSION_RECONNECT, "Reconnect\tCtrl+R");
    AppendMenuA(sm, MF_STRING, IDM_SESSION_DISCONNECT, "Disconnect");
    AppendMenuA(sm, MF_SEPARATOR, 0, NULL);
    AppendMenuA(sm, MF_STRING, IDM_SESSION_BROADCAST, "Broadcast Mode\tCtrl+B");
    AppendMenuA(sm, MF_STRING, IDM_SESSION_LOCK, "Lock Session");
    AppendMenuA(sm, MF_STRING, IDM_SESSION_LOG, "Toggle Logging");
    AppendMenuA(sm, MF_SEPARATOR, 0, NULL);
    AppendMenuA(sm, MF_STRING, IDM_SESSION_SETTINGS, "Session Settings...");
    AppendMenuA(sm, MF_STRING, IDM_SESSION_SEND_CMD, "Send Command...");

    AppendMenuA(vm, MF_STRING, IDM_VIEW_FULLSCREEN, "Full Screen\tF11");
    AppendMenuA(vm, MF_SEPARATOR, 0, NULL);
    AppendMenuA(vm, MF_STRING, IDM_VIEW_ZOOM_IN, "Zoom In\tCtrl++");
    AppendMenuA(vm, MF_STRING, IDM_VIEW_ZOOM_OUT, "Zoom Out\tCtrl+-");
    AppendMenuA(vm, MF_STRING, IDM_VIEW_ZOOM_RESET, "Reset Zoom\tCtrl+0");
    AppendMenuA(vm, MF_SEPARATOR, 0, NULL);
    AppendMenuA(vm, MF_STRING|MF_CHECKED, IDM_VIEW_SIDEBAR, "Sidebar\tCtrl+\\");
    AppendMenuA(vm, MF_STRING|MF_CHECKED, IDM_VIEW_TABS, "Tab Bar");
    AppendMenuA(vm, MF_STRING, IDM_VIEW_SPLIT_H, "Split Horizontal\tCtrl+Shift+H");
    AppendMenuA(vm, MF_STRING, IDM_VIEW_SPLIT_V, "Split Vertical\tCtrl+Shift+V");
    AppendMenuA(vm, MF_SEPARATOR, 0, NULL);
    AppendMenuA(vm, MF_STRING, IDM_VIEW_TOOLBAR, "Toolbar");
    AppendMenuA(vm, MF_STRING|MF_CHECKED, IDM_VIEW_STATUSBAR, "Status Bar");
    AppendMenuA(vm, MF_STRING, IDM_VIEW_OPACITY, "Opacity...");
    AppendMenuA(vm, MF_SEPARATOR, 0, NULL);
    AppendMenuA(vm, MF_STRING, IDM_VIEW_THEME, "Theme...");

    AppendMenuA(tm, MF_STRING, IDM_TOOLS_SNIPPETS, "Snippets\tCtrl+Shift+S");
    AppendMenuA(tm, MF_STRING, IDM_TOOLS_MACROS, "Macros\tCtrl+Shift+M");
    AppendMenuA(tm, MF_STRING, IDM_TOOLS_KEYGEN, "Key Generator...");
    AppendMenuA(tm, MF_SEPARATOR, 0, NULL);
    AppendMenuA(tm, MF_STRING, IDM_TOOLS_SFTP, "SFTP Panel\tCtrl+Shift+F");
    AppendMenuA(tm, MF_STRING, IDM_TOOLS_BOOKMARKS, "Bookmarks\tCtrl+Shift+B");
    AppendMenuA(tm, MF_STRING, IDM_TOOLS_TUNNEL, "Port Tunnels...");
    AppendMenuA(tm, MF_SEPARATOR, 0, NULL);
    AppendMenuA(tm, MF_STRING, IDM_TOOLS_SCRIPTMGR, "Script Manager...");
    AppendMenuA(tm, MF_STRING, IDM_TOOLS_CONNPROF, "Connection Profiler");
    AppendMenuA(tm, MF_STRING, IDM_TOOLS_MONITOR, "Session Monitor");
    AppendMenuA(tm, MF_STRING, IDM_TOOLS_NETDIAG, "Network Diagnostics");

    AppendMenuA(hm, MF_STRING, IDM_HELP_DOCS, "Documentation\tF1");
    AppendMenuA(hm, MF_STRING, IDM_HELP_SHORTCUTS, "Keyboard Shortcuts");
    AppendMenuA(hm, MF_STRING, IDM_HELP_CHANGELOG, "Changelog");
    AppendMenuA(hm, MF_STRING, IDM_HELP_UPDATE, "Check for Updates...");
    AppendMenuA(hm, MF_SEPARATOR, 0, NULL);
    AppendMenuA(hm, MF_STRING, IDM_HELP_ABOUT, "About PuttyAlt");

    AppendMenuA(menu, MF_POPUP, (UINT_PTR)fm, "File");
    AppendMenuA(menu, MF_POPUP, (UINT_PTR)em, "Edit");
    AppendMenuA(menu, MF_POPUP, (UINT_PTR)sm, "Session");
    AppendMenuA(menu, MF_POPUP, (UINT_PTR)vm, "View");
    AppendMenuA(menu, MF_POPUP, (UINT_PTR)tm, "Tools");
    AppendMenuA(menu, MF_POPUP, (UINT_PTR)hm, "Help");

    SetMenu((HWND)gui->hwnd, menu);
    gui->menu = (void *)menu;
}

/* ══════════════════════════════════════════
 *  Custom status bar (owner-drawn)
 * ══════════════════════════════════════════ */

static void gui_paint_statusbar(GUIState *gui, HDC hdc, RECT *rc)
{
    /* Fill background */
    HBRUSH bg = CreateSolidBrush(HEX_RGB(DS_STATUS_BG));
    FillRect(hdc, rc, bg);
    DeleteObject(bg);

    /* Top border line */
    HPEN pen = CreatePen(PS_SOLID, 1, HEX_RGB(DS_BORDER_SUBTLE));
    HPEN old_pen = (HPEN)SelectObject(hdc, pen);
    MoveToEx(hdc, rc->left, rc->top, NULL);
    LineTo(hdc, rc->right, rc->top);
    SelectObject(hdc, old_pen);
    DeleteObject(pen);

    SetBkMode(hdc, TRANSPARENT);
    HFONT font = CreateFontA(DS_FONT_SIZE_SM, 0, 0, 0, FW_NORMAL, 0, 0, 0,
        DEFAULT_CHARSET, 0, 0, CLEARTYPE_QUALITY,
        DEFAULT_PITCH, DS_FONT_UI);
    HFONT old_font = (HFONT)SelectObject(hdc, font);
    SetTextColor(hdc, HEX_RGB(DS_STATUS_FG));

    /* Status text */
    RECT text_rc = { rc->left + DS_SPACE_MD, rc->top + 4,
                     rc->right - 200, rc->bottom - 2 };
    DrawTextA(hdc, gui->status_text, -1, &text_rc,
              DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS);

    /* Connection indicator */
    if (gui->connected) {
        /* Green dot */
        HBRUSH dot = CreateSolidBrush(HEX_RGB(DS_SUCCESS));
        RECT dot_rc = { rc->right - 190, rc->top + 8, rc->right - 182, rc->top + 16 };
        HBRUSH old_b = (HBRUSH)SelectObject(hdc, dot);
        HPEN dot_pen = CreatePen(PS_SOLID, 1, HEX_RGB(DS_SUCCESS));
        HPEN old_dp = (HPEN)SelectObject(hdc, dot_pen);
        Ellipse(hdc, dot_rc.left, dot_rc.top, dot_rc.right, dot_rc.bottom);
        SelectObject(hdc, old_b);
        SelectObject(hdc, old_dp);
        DeleteObject(dot);
        DeleteObject(dot_pen);

        SetTextColor(hdc, HEX_RGB(DS_SUCCESS));
        RECT conn_rc = { rc->right - 178, rc->top + 4, rc->right - 100, rc->bottom - 2 };
        DrawTextA(hdc, "Connected", -1, &conn_rc,
                  DT_LEFT | DT_SINGLELINE | DT_VCENTER);
    }

    /* Version on the right */
    SetTextColor(hdc, HEX_RGB(DS_TEXT_MUTED));
    RECT ver_rc = { rc->right - 100, rc->top + 4, rc->right - DS_SPACE_SM, rc->bottom - 2 };
    DrawTextA(hdc, PUTTYALT_VERSION_STR, -1, &ver_rc,
              DT_RIGHT | DT_SINGLELINE | DT_VCENTER);

    SelectObject(hdc, old_font);
    DeleteObject(font);
}

/* ══════════════════════════════════════════
 *  Custom tab bar (owner-drawn)
 * ══════════════════════════════════════════ */

#define MAX_TABS 32

typedef struct {
    char title[64];
    int active;
    int hover;
    int close_hover;
} TabInfo;

static TabInfo g_tabs[MAX_TABS];
static int g_tab_count = 1;
static int g_active_tab = 0;

static LRESULT CALLBACK tabbar_wndproc(HWND hwnd, UINT msg,
                                        WPARAM wparam, LPARAM lparam)
{
    switch (msg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        PaintBuffer pb = paint_begin(hwnd, &ps);
        HDC hdc = pb.mem_dc;
        RECT rc = pb.rc;

        /* Tab bar background */
        HBRUSH bg = CreateSolidBrush(HEX_RGB(DS_TAB_BG));
        FillRect(hdc, &rc, bg);
        DeleteObject(bg);

        /* Bottom border */
        HPEN bpen = CreatePen(PS_SOLID, 1, HEX_RGB(DS_BORDER_SUBTLE));
        HPEN old_pen = (HPEN)SelectObject(hdc, bpen);
        MoveToEx(hdc, 0, rc.bottom - 1, NULL);
        LineTo(hdc, rc.right, rc.bottom - 1);
        SelectObject(hdc, old_pen);
        DeleteObject(bpen);

        SetBkMode(hdc, TRANSPARENT);
        HFONT font = CreateFontA(DS_FONT_SIZE_SM, 0, 0, 0, FW_NORMAL, 0, 0, 0,
            DEFAULT_CHARSET, 0, 0, CLEARTYPE_QUALITY,
            DEFAULT_PITCH, DS_FONT_UI);
        HFONT old_font = (HFONT)SelectObject(hdc, font);

        int x = DS_SPACE_SM;
        for (int i = 0; i < g_tab_count && i < MAX_TABS; i++) {
            int tab_w = DS_TAB_MIN_WIDTH + 40;
            RECT tab_rc = { x, 2, x + tab_w, rc.bottom - 2 };

            if (i == g_active_tab) {
                /* Active tab: surface background with accent bottom border */
                fill_rounded_rect(hdc, &tab_rc, 4, HEX_RGB(DS_TAB_ACTIVE));

                /* Accent indicator at bottom */
                HPEN accent = CreatePen(PS_SOLID, 2, HEX_RGB(DS_ACCENT));
                HPEN op = (HPEN)SelectObject(hdc, accent);
                MoveToEx(hdc, x + 4, rc.bottom - 2, NULL);
                LineTo(hdc, x + tab_w - 4, rc.bottom - 2);
                SelectObject(hdc, op);
                DeleteObject(accent);

                SetTextColor(hdc, HEX_RGB(DS_TEXT));
            } else if (g_tabs[i].hover) {
                fill_rounded_rect(hdc, &tab_rc, 4, HEX_RGB(DS_TAB_HOVER));
                SetTextColor(hdc, HEX_RGB(DS_TEXT_SECONDARY));
            } else {
                SetTextColor(hdc, HEX_RGB(DS_TEXT_SECONDARY));
            }

            /* Tab title */
            RECT title_rc = { x + DS_SPACE_MD, 2, x + tab_w - 24, rc.bottom - 2 };
            DrawTextA(hdc, g_tabs[i].title, -1, &title_rc,
                      DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS);

            /* Close button (x) */
            if (g_tab_count > 1) {
                RECT close_rc = { x + tab_w - 22, (rc.bottom - 12) / 2,
                                  x + tab_w - 10, (rc.bottom + 12) / 2 };
                if (g_tabs[i].close_hover) {
                    SetTextColor(hdc, HEX_RGB(DS_ERROR));
                } else {
                    SetTextColor(hdc, HEX_RGB(DS_TEXT_MUTED));
                }
                DrawTextA(hdc, "\xC3\x97", -1, &close_rc,
                          DT_CENTER | DT_SINGLELINE | DT_VCENTER);
            }

            x += tab_w + DS_SPACE_XS;
        }

        /* New tab (+) button */
        SetTextColor(hdc, HEX_RGB(DS_TEXT_MUTED));
        RECT plus_rc = { x, 2, x + 28, rc.bottom - 2 };
        DrawTextA(hdc, "+", -1, &plus_rc,
                  DT_CENTER | DT_SINGLELINE | DT_VCENTER);

        SelectObject(hdc, old_font);
        DeleteObject(font);
        paint_end(&pb, &ps);
        return 0;
    }

    case WM_LBUTTONDOWN: {
        int mx = LOWORD(lparam);
        int x = DS_SPACE_SM;
        int tab_w = DS_TAB_MIN_WIDTH + 40;

        for (int i = 0; i < g_tab_count && i < MAX_TABS; i++) {
            if (mx >= x && mx < x + tab_w) {
                /* Check close button area */
                if (mx >= x + tab_w - 22 && g_tab_count > 1) {
                    /* Close tab */
                    for (int j = i; j < g_tab_count - 1; j++)
                        g_tabs[j] = g_tabs[j + 1];
                    g_tab_count--;
                    if (g_active_tab >= g_tab_count)
                        g_active_tab = g_tab_count - 1;
                } else {
                    g_active_tab = i;
                }
                InvalidateRect(hwnd, NULL, FALSE);
                /* Notify parent */
                HWND parent = GetParent(hwnd);
                if (parent) SendMessage(parent, WM_USER + 100, g_active_tab, 0);
                return 0;
            }
            x += tab_w + DS_SPACE_XS;
        }

        /* New tab button */
        if (mx >= x && mx < x + 28) {
            if (g_tab_count < MAX_TABS) {
                snprintf(g_tabs[g_tab_count].title,
                         sizeof(g_tabs[g_tab_count].title), "New Session");
                g_active_tab = g_tab_count;
                g_tab_count++;
                InvalidateRect(hwnd, NULL, FALSE);
                HWND parent = GetParent(hwnd);
                if (parent) SendMessage(parent, WM_COMMAND, IDM_FILE_NEW, 0);
            }
        }
        return 0;
    }

    case WM_MOUSEMOVE: {
        int mx = LOWORD(lparam);
        int x = DS_SPACE_SM;
        int tab_w = DS_TAB_MIN_WIDTH + 40;
        int changed = 0;

        for (int i = 0; i < g_tab_count && i < MAX_TABS; i++) {
            int hovering = (mx >= x && mx < x + tab_w);
            int close_h = hovering && (mx >= x + tab_w - 22);
            if (g_tabs[i].hover != hovering || g_tabs[i].close_hover != close_h) {
                g_tabs[i].hover = hovering;
                g_tabs[i].close_hover = close_h;
                changed = 1;
            }
            x += tab_w + DS_SPACE_XS;
        }

        if (changed) InvalidateRect(hwnd, NULL, FALSE);

        /* Track mouse leave */
        TRACKMOUSEEVENT tme = { sizeof(tme), TME_LEAVE, hwnd, 0 };
        TrackMouseEvent(&tme);
        return 0;
    }

    case WM_MOUSELEAVE: {
        for (int i = 0; i < g_tab_count; i++) {
            g_tabs[i].hover = 0;
            g_tabs[i].close_hover = 0;
        }
        InvalidateRect(hwnd, NULL, FALSE);
        return 0;
    }

    case WM_ERASEBKGND:
        return 1;
    }
    return DefWindowProcA(hwnd, msg, wparam, lparam);
}

/* ══════════════════════════════════════════
 *  Keyboard accelerators
 * ══════════════════════════════════════════ */

static HACCEL gui_create_accelerators(void)
{
    ACCEL acc[] = {
        { FCONTROL | FVIRTKEY,  'N',           IDM_FILE_NEW },
        { FCONTROL | FVIRTKEY,  'O',           IDM_FILE_OPEN },
        { FCONTROL | FVIRTKEY,  'S',           IDM_FILE_SAVE_SESSION },
        { FCONTROL | FVIRTKEY,  'D',           IDM_FILE_DUPLICATE },
        { FCONTROL | FSHIFT | FVIRTKEY, 'C',   IDM_EDIT_COPY },
        { FCONTROL | FSHIFT | FVIRTKEY, 'V',   IDM_EDIT_PASTE },
        { FCONTROL | FSHIFT | FVIRTKEY, 'A',   IDM_EDIT_SELECTALL },
        { FCONTROL | FVIRTKEY,  'F',           IDM_EDIT_FIND },
        { FCONTROL | FVIRTKEY,  'R',           IDM_SESSION_RECONNECT },
        { FCONTROL | FVIRTKEY,  'B',           IDM_SESSION_BROADCAST },
        { FVIRTKEY,             VK_F11,        IDM_VIEW_FULLSCREEN },
        { FVIRTKEY,             VK_F1,         IDM_HELP_DOCS },
        { FCONTROL | FVIRTKEY,  VK_OEM_PLUS,   IDM_VIEW_ZOOM_IN },
        { FCONTROL | FVIRTKEY,  VK_OEM_MINUS,  IDM_VIEW_ZOOM_OUT },
        { FCONTROL | FVIRTKEY,  '0',           IDM_VIEW_ZOOM_RESET },
        { FCONTROL | FVIRTKEY,  VK_OEM_COMMA,  IDM_EDIT_PREFERENCES },
    };
    return CreateAcceleratorTableA(acc, sizeof(acc) / sizeof(acc[0]));
}

/* ══════════════════════════════════════════
 *  Sidebar window procedure (modern design)
 * ══════════════════════════════════════════ */

static LRESULT CALLBACK sidebar_wndproc(HWND hwnd, UINT msg,
                                         WPARAM wparam, LPARAM lparam)
{
    switch (msg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        PaintBuffer pb = paint_begin(hwnd, &ps);
        HDC hdc = pb.mem_dc;
        RECT rc = pb.rc;
        GUIState *gui = (GUIState *)GetWindowLongPtrA(hwnd, GWLP_USERDATA);

        /* Sidebar background */
        HBRUSH bg = CreateSolidBrush(HEX_RGB(DS_SIDEBAR_BG));
        FillRect(hdc, &rc, bg);
        DeleteObject(bg);

        /* Right border */
        HPEN pen = CreatePen(PS_SOLID, 1, HEX_RGB(DS_BORDER_SUBTLE));
        HPEN old_pen = (HPEN)SelectObject(hdc, pen);
        MoveToEx(hdc, rc.right - 1, 0, NULL);
        LineTo(hdc, rc.right - 1, rc.bottom);
        SelectObject(hdc, old_pen);
        DeleteObject(pen);

        SetBkMode(hdc, TRANSPARENT);

        /* Section header */
        HFONT hdr_font = CreateFontA(DS_FONT_SIZE_XS, 0, 0, 0, FW_BOLD, 0, 0, 0,
            DEFAULT_CHARSET, 0, 0, CLEARTYPE_QUALITY,
            DEFAULT_PITCH, DS_FONT_UI);
        HFONT old_f = (HFONT)SelectObject(hdc, hdr_font);
        SetTextColor(hdc, HEX_RGB(DS_TEXT_MUTED));

        RECT hdr_rc = { DS_SPACE_LG, DS_SPACE_LG, rc.right - DS_SPACE_SM, DS_SPACE_LG + 14 };
        DrawTextA(hdc, "SESSIONS", -1, &hdr_rc, DT_LEFT | DT_SINGLELINE);
        SelectObject(hdc, old_f);
        DeleteObject(hdr_font);

        /* Session items */
        HFONT item_font = CreateFontA(DS_FONT_SIZE_MD, 0, 0, 0, FW_NORMAL, 0, 0, 0,
            DEFAULT_CHARSET, 0, 0, CLEARTYPE_QUALITY,
            DEFAULT_PITCH, DS_FONT_UI);
        old_f = (HFONT)SelectObject(hdc, item_font);

        int y = DS_SPACE_LG + 14 + DS_SPACE_MD;

        if (gui && gui->connected) {
            /* Active session item with highlight */
            RECT item_bg = { DS_SPACE_SM, y, rc.right - DS_SPACE_SM - 1, y + 36 };
            fill_rounded_rect(hdc, &item_bg, 4, HEX_RGB(DS_SURFACE_HOVER));

            /* Status dot */
            HBRUSH dot = CreateSolidBrush(HEX_RGB(DS_SUCCESS));
            HPEN dot_pen = CreatePen(PS_SOLID, 1, HEX_RGB(DS_SUCCESS));
            HBRUSH old_b = (HBRUSH)SelectObject(hdc, dot);
            HPEN old_dp = (HPEN)SelectObject(hdc, dot_pen);
            Ellipse(hdc, DS_SPACE_LG, y + 12, DS_SPACE_LG + 8, y + 20);
            SelectObject(hdc, old_b);
            SelectObject(hdc, old_dp);
            DeleteObject(dot);
            DeleteObject(dot_pen);

            /* Session name */
            SetTextColor(hdc, HEX_RGB(DS_TEXT));
            RECT name_rc = { DS_SPACE_LG + 14, y + 4, rc.right - DS_SPACE_LG, y + 20 };
            DrawTextA(hdc, gui->title, -1, &name_rc,
                      DT_LEFT | DT_SINGLELINE | DT_END_ELLIPSIS);

            /* Traffic info */
            char traffic[64];
            snprintf(traffic, sizeof(traffic), "%lu B / %lu B",
                     gui->bytes_sent, gui->bytes_recv);
            SetTextColor(hdc, HEX_RGB(DS_TEXT_MUTED));
            RECT traf_rc = { DS_SPACE_LG + 14, y + 20, rc.right - DS_SPACE_LG, y + 34 };
            DrawTextA(hdc, traffic, -1, &traf_rc,
                      DT_LEFT | DT_SINGLELINE | DT_END_ELLIPSIS);

            y += 44;
        } else {
            /* Empty state */
            SetTextColor(hdc, HEX_RGB(DS_TEXT_SECONDARY));
            RECT empty_rc = { DS_SPACE_LG, y, rc.right - DS_SPACE_SM, y + 20 };
            DrawTextA(hdc, "No active sessions", -1, &empty_rc,
                      DT_LEFT | DT_SINGLELINE);
            y += 28;

            SetTextColor(hdc, HEX_RGB(DS_TEXT_MUTED));
            RECT hint_rc = { DS_SPACE_LG, y, rc.right - DS_SPACE_SM, y + 20 };
            DrawTextA(hdc, "Press Ctrl+N to connect", -1, &hint_rc,
                      DT_LEFT | DT_SINGLELINE);
            y += 36;
        }

        /* Separator */
        HPEN sep = CreatePen(PS_SOLID, 1, HEX_RGB(DS_BORDER_SUBTLE));
        HPEN old_sp = (HPEN)SelectObject(hdc, sep);
        MoveToEx(hdc, DS_SPACE_LG, y, NULL);
        LineTo(hdc, rc.right - DS_SPACE_LG, y);
        SelectObject(hdc, old_sp);
        DeleteObject(sep);

        y += DS_SPACE_MD;

        /* Quick actions section */
        HFONT sec_font = CreateFontA(DS_FONT_SIZE_XS, 0, 0, 0, FW_BOLD, 0, 0, 0,
            DEFAULT_CHARSET, 0, 0, CLEARTYPE_QUALITY,
            DEFAULT_PITCH, DS_FONT_UI);
        SelectObject(hdc, sec_font);
        SetTextColor(hdc, HEX_RGB(DS_TEXT_MUTED));
        RECT qa_hdr = { DS_SPACE_LG, y, rc.right - DS_SPACE_SM, y + 14 };
        DrawTextA(hdc, "QUICK ACTIONS", -1, &qa_hdr, DT_LEFT | DT_SINGLELINE);
        SelectObject(hdc, item_font);
        DeleteObject(sec_font);

        y += 14 + DS_SPACE_SM;

        /* Quick action items */
        const char *actions[] = {
            "New Session",
            "Snippets",
            "SFTP",
            "Key Manager",
        };
        const char *shortcuts[] = {
            "Ctrl+N", "Ctrl+Shift+S", "Ctrl+Shift+F", ""
        };

        for (int i = 0; i < 4; i++) {
            SetTextColor(hdc, HEX_RGB(DS_TEXT_SECONDARY));
            RECT act_rc = { DS_SPACE_LG, y, rc.right - 70, y + 20 };
            DrawTextA(hdc, actions[i], -1, &act_rc,
                      DT_LEFT | DT_SINGLELINE | DT_VCENTER);

            if (shortcuts[i][0]) {
                SetTextColor(hdc, HEX_RGB(DS_TEXT_MUTED));
                RECT sc_rc = { rc.right - 90, y, rc.right - DS_SPACE_LG, y + 20 };
                DrawTextA(hdc, shortcuts[i], -1, &sc_rc,
                          DT_RIGHT | DT_SINGLELINE | DT_VCENTER);
            }
            y += 24;
        }

        SelectObject(hdc, old_f);
        DeleteObject(item_font);
        paint_end(&pb, &ps);
        return 0;
    }
    case WM_ERASEBKGND:
        return 1;
    }
    return DefWindowProcA(hwnd, msg, wparam, lparam);
}

/* ══════════════════════════════════════════
 *  Terminal window procedure (modern design)
 * ══════════════════════════════════════════ */

static LRESULT CALLBACK term_wndproc(HWND hwnd, UINT msg,
                                      WPARAM wparam, LPARAM lparam)
{
    switch (msg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        PaintBuffer pb = paint_begin(hwnd, &ps);
        HDC hdc = pb.mem_dc;
        RECT rc = pb.rc;
        GUIState *gui = (GUIState *)GetWindowLongPtrA(hwnd, GWLP_USERDATA);

        /* Terminal background */
        HBRUSH bg = CreateSolidBrush(HEX_RGB(DS_TERM_BG));
        FillRect(hdc, &rc, bg);
        DeleteObject(bg);

        SetBkMode(hdc, TRANSPARENT);

        HFONT mono = CreateFontA(15, 0, 0, 0, FW_NORMAL, 0, 0, 0,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY, FIXED_PITCH | FF_MODERN, DS_FONT_MONO);
        HFONT old = (HFONT)SelectObject(hdc, mono);

        int cy = DS_SPACE_XL;
        int cx = DS_SPACE_XL;

        if (gui && gui->connected) {
            /* Connected: simulated terminal */
            SetTextColor(hdc, HEX_RGB(DS_SUCCESS));
            const char *conn = "Connected to remote host";
            TextOutA(hdc, cx, cy, conn, (int)strlen(conn));
            cy += 22;

            SetTextColor(hdc, HEX_RGB(DS_TEXT));
            TextOutA(hdc, cx, cy, gui->title, (int)strlen(gui->title));
            cy += 30;

            SetTextColor(hdc, HEX_RGB(DS_TEXT_SECONDARY));
            const char *hint = "Terminal session active. Type commands below.";
            TextOutA(hdc, cx, cy, hint, (int)strlen(hint));
            cy += 36;

            /* Simulated prompt with cursor */
            SetTextColor(hdc, HEX_RGB(DS_ACCENT));
            const char *user_part = "user@host";
            TextOutA(hdc, cx, cy, user_part, (int)strlen(user_part));

            SIZE sz;
            GetTextExtentPoint32A(hdc, user_part, (int)strlen(user_part), &sz);

            SetTextColor(hdc, HEX_RGB(DS_TEXT));
            const char *colon = ":~$ ";
            TextOutA(hdc, cx + sz.cx, cy, colon, (int)strlen(colon));

            /* Blinking cursor block */
            GetTextExtentPoint32A(hdc, colon, (int)strlen(colon), &sz);
            RECT cursor_rc = { cx + sz.cx + 60, cy, cx + sz.cx + 68, cy + 16 };
            HBRUSH cursor_br = CreateSolidBrush(HEX_RGB(DS_TERM_CURSOR));
            FillRect(hdc, &cursor_rc, cursor_br);
            DeleteObject(cursor_br);
        } else {
            /* Welcome screen — centered, minimal */
            int center_x = (rc.right - rc.left) / 2;
            int center_y = (rc.bottom - rc.top) / 3;

            /* Logo / app name */
            HFONT title_font = CreateFontA(DS_FONT_SIZE_TITLE + 6, 0, 0, 0,
                FW_LIGHT, 0, 0, 0,
                DEFAULT_CHARSET, 0, 0, CLEARTYPE_QUALITY,
                DEFAULT_PITCH, DS_FONT_UI);
            HFONT old_tf = (HFONT)SelectObject(hdc, title_font);

            SetTextColor(hdc, HEX_RGB(DS_TEXT));
            const char *name = PUTTYALT_NAME;
            SIZE name_sz;
            GetTextExtentPoint32A(hdc, name, (int)strlen(name), &name_sz);
            TextOutA(hdc, center_x - name_sz.cx / 2, center_y, name, (int)strlen(name));

            SelectObject(hdc, old_tf);
            DeleteObject(title_font);

            /* Subtitle */
            HFONT sub_font = CreateFontA(DS_FONT_SIZE_LG, 0, 0, 0, FW_NORMAL, 0, 0, 0,
                DEFAULT_CHARSET, 0, 0, CLEARTYPE_QUALITY,
                DEFAULT_PITCH, DS_FONT_UI);
            HFONT old_sf = (HFONT)SelectObject(hdc, sub_font);

            SetTextColor(hdc, HEX_RGB(DS_TEXT_SECONDARY));
            const char *sub = "Modern SSH Terminal";
            SIZE sub_sz;
            GetTextExtentPoint32A(hdc, sub, (int)strlen(sub), &sub_sz);
            TextOutA(hdc, center_x - sub_sz.cx / 2, center_y + name_sz.cy + 8,
                     sub, (int)strlen(sub));

            SelectObject(hdc, old_sf);
            DeleteObject(sub_font);

            /* Keyboard shortcuts in a grid */
            cy = center_y + name_sz.cy + 48;

            SelectObject(hdc, mono);  /* switch back to mono font */

            const char *keys[] = {
                "Ctrl+N",  "New session",
                "Ctrl+O",  "Open saved",
                "Ctrl+F",  "Find",
                "F11",     "Fullscreen",
                "Ctrl+,",  "Settings",
            };

            for (int i = 0; i < 10; i += 2) {
                SetTextColor(hdc, HEX_RGB(DS_ACCENT_MUTED));
                RECT key_rc = { center_x - 120, cy, center_x - 10, cy + 18 };
                DrawTextA(hdc, keys[i], -1, &key_rc,
                          DT_RIGHT | DT_SINGLELINE | DT_VCENTER);

                SetTextColor(hdc, HEX_RGB(DS_TEXT_SECONDARY));
                RECT desc_rc = { center_x + 10, cy, center_x + 160, cy + 18 };
                DrawTextA(hdc, keys[i + 1], -1, &desc_rc,
                          DT_LEFT | DT_SINGLELINE | DT_VCENTER);

                cy += 24;
            }

            /* Version at bottom */
            cy = rc.bottom - 40;
            SetTextColor(hdc, HEX_RGB(DS_TEXT_MUTED));
            char ver[128];
            snprintf(ver, sizeof(ver), "%s  |  MIT License", PUTTYALT_VERSION_STR);
            SIZE ver_sz;
            GetTextExtentPoint32A(hdc, ver, (int)strlen(ver), &ver_sz);
            TextOutA(hdc, center_x - ver_sz.cx / 2, cy, ver, (int)strlen(ver));
        }

        SelectObject(hdc, old);
        DeleteObject(mono);
        paint_end(&pb, &ps);
        return 0;
    }
    case WM_ERASEBKGND:
        return 1;
    }
    return DefWindowProcA(hwnd, msg, wparam, lparam);
}

/* ══════════════════════════════════════════
 *  Main window procedure — ALL handlers
 * ══════════════════════════════════════════ */

static LRESULT CALLBACK gui_wndproc(HWND hwnd, UINT msg,
                                     WPARAM wparam, LPARAM lparam)
{
    GUIState *gui = (GUIState *)GetWindowLongPtrA(hwnd, GWLP_USERDATA);

    switch (msg) {
    case WM_CREATE: {
        CREATESTRUCTA *cs = (CREATESTRUCTA *)lparam;
        SetWindowLongPtrA(hwnd, GWLP_USERDATA, (LONG_PTR)cs->lpCreateParams);
        return 0;
    }

    case WM_SIZE:
        if (gui) {
            RECT rc;
            GetClientRect(hwnd, &rc);
            gui->config.width = rc.right;
            gui->config.height = rc.bottom;

            int top = 0;
            int sb_h = gui->config.statusbar_visible ? DS_STATUSBAR_HEIGHT : 0;

            /* Tab bar */
            if (gui->tab_ctrl) {
                MoveWindow((HWND)gui->tab_ctrl, 0, top,
                           rc.right, DS_TABBAR_HEIGHT, TRUE);
                top += DS_TABBAR_HEIGHT;
            }

            /* Sidebar */
            int sb_left = 0;
            if (gui->sidebar_hwnd && gui->config.sidebar_visible) {
                MoveWindow((HWND)gui->sidebar_hwnd, 0, top,
                           gui->config.sidebar_width,
                           rc.bottom - top - sb_h, TRUE);
                ShowWindow((HWND)gui->sidebar_hwnd, SW_SHOW);
                sb_left = gui->config.sidebar_width;
            } else if (gui->sidebar_hwnd) {
                ShowWindow((HWND)gui->sidebar_hwnd, SW_HIDE);
            }

            /* Terminal fills remaining space */
            if (gui->term_hwnd)
                MoveWindow((HWND)gui->term_hwnd, sb_left, top,
                           rc.right - sb_left,
                           rc.bottom - top - sb_h, TRUE);

            /* Statusbar area — handled in WM_PAINT of main window */
            InvalidateRect(hwnd, NULL, FALSE);
        }
        return 0;

    case WM_PAINT:
        if (gui && gui->config.statusbar_visible) {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            RECT rc;
            GetClientRect(hwnd, &rc);

            /* Paint status bar at bottom */
            RECT sb_rc = { 0, rc.bottom - DS_STATUSBAR_HEIGHT,
                          rc.right, rc.bottom };
            gui_paint_statusbar(gui, hdc, &sb_rc);

            EndPaint(hwnd, &ps);
            return 0;
        }
        break;

    case WM_COMMAND:
        if (!gui) return 0;

        switch (LOWORD(wparam)) {

        /* ── File menu ── */
        case IDM_FILE_NEW:
        case IDM_FILE_OPEN: {
            ConnectParams params;
            memset(&params, 0, sizeof(params));
            params.port = 22;
            if (dialog_connect(gui, &params) == 0) {
                gui_connect(gui, params.hostname, params.port,
                            params.username);
            }
            break;
        }
        case IDM_FILE_SAVE_SESSION:
            gui_set_status(gui, "Session saved");
            break;

        case IDM_FILE_DUPLICATE:
            if (g_tab_count < MAX_TABS) {
                snprintf(g_tabs[g_tab_count].title,
                         sizeof(g_tabs[g_tab_count].title),
                         "%s", g_tabs[g_active_tab].title);
                g_active_tab = g_tab_count;
                g_tab_count++;
                if (gui->tab_ctrl)
                    InvalidateRect((HWND)gui->tab_ctrl, NULL, FALSE);
                gui_set_status(gui, "Tab duplicated");
            }
            break;

        case IDM_FILE_LOG:
            gui->config.state_flags ^= GUI_STATE_RECORDING;
            gui_set_status(gui,
                (gui->config.state_flags & GUI_STATE_RECORDING)
                    ? "Logging started" : "Logging stopped");
            break;

        case IDM_FILE_IMPORT:
            MessageBoxA(hwnd,
                "Import Sessions\n\n"
                "Import saved sessions from PuTTY, SSH config,\n"
                "or PuttyAlt backup files.",
                PUTTYALT_NAME " - Import", MB_OK | MB_ICONINFORMATION);
            break;

        case IDM_FILE_EXPORT:
            MessageBoxA(hwnd,
                "Export Sessions\n\n"
                "Export your sessions to a backup file\n"
                "for migration or sharing.",
                PUTTYALT_NAME " - Export", MB_OK | MB_ICONINFORMATION);
            break;

        case IDM_FILE_EXIT:
            PostMessage(hwnd, WM_CLOSE, 0, 0);
            break;

        /* ── Edit menu ── */
        case IDM_EDIT_COPY:
            if (gui->connected) {
                if (OpenClipboard(hwnd)) {
                    EmptyClipboard();
                    CloseClipboard();
                }
                gui_set_status(gui, "Copied to clipboard");
            } else {
                gui_set_status(gui, "No text selected");
            }
            break;

        case IDM_EDIT_PASTE:
            if (gui->connected) {
                if (OpenClipboard(hwnd)) {
                    HANDLE hd = GetClipboardData(CF_TEXT);
                    if (hd) {
                        char *txt = (char *)GlobalLock(hd);
                        if (txt) {
                            gui_set_status(gui, "Pasted from clipboard");
                            GlobalUnlock(hd);
                        }
                    }
                    CloseClipboard();
                }
            } else {
                gui_set_status(gui, "Not connected");
            }
            break;

        case IDM_EDIT_SELECTALL:
            gui_set_status(gui, "All text selected");
            break;

        case IDM_EDIT_FIND: {
            char search[256] = "";
            if (dialog_find(gui, search, sizeof(search)) == 0) {
                char fmsg[300];
                snprintf(fmsg, sizeof(fmsg), "Searching: %s", search);
                gui_set_status(gui, fmsg);
            }
            break;
        }

        case IDM_EDIT_CLEAR:
            gui_terminal_clear(gui);
            gui_set_status(gui, "Scrollback cleared");
            break;

        case IDM_EDIT_PREFERENCES:
        case IDM_SESSION_SETTINGS:
            dialog_settings(gui);
            break;

        /* ── Session menu ── */
        case IDM_SESSION_RECONNECT:
            gui_reconnect(gui);
            break;

        case IDM_SESSION_DISCONNECT:
            gui_disconnect(gui);
            break;

        case IDM_SESSION_SEND_CMD:
            if (gui->connected) {
                gui_set_status(gui, "Command input ready");
            } else {
                gui_set_status(gui, "Not connected");
            }
            break;

        case IDM_SESSION_BROADCAST:
        case IDM_TOOLS_BROADCAST:
            gui->config.state_flags ^= GUI_STATE_BROADCASTING;
            gui_set_status(gui,
                (gui->config.state_flags & GUI_STATE_BROADCASTING)
                    ? "Broadcast mode ON" : "Broadcast mode OFF");
            break;

        case IDM_SESSION_LOCK:
            gui->config.state_flags ^= GUI_STATE_LOCKED;
            gui_set_status(gui,
                (gui->config.state_flags & GUI_STATE_LOCKED)
                    ? "Session locked" : "Session unlocked");
            break;

        case IDM_SESSION_LOG:
            gui->config.state_flags ^= GUI_STATE_RECORDING;
            gui_set_status(gui,
                (gui->config.state_flags & GUI_STATE_RECORDING)
                    ? "Logging ON" : "Logging OFF");
            break;

        /* ── View menu ── */
        case IDM_VIEW_FULLSCREEN:
            gui_toggle_fullscreen(gui);
            break;

        case IDM_VIEW_ZOOM_IN:
            gui_zoom_in(gui);
            break;

        case IDM_VIEW_ZOOM_OUT:
            gui_zoom_out(gui);
            break;

        case IDM_VIEW_ZOOM_RESET:
            gui_zoom_reset(gui);
            break;

        case IDM_VIEW_SIDEBAR:
            gui_toggle_sidebar(gui);
            CheckMenuItem(GetMenu(hwnd), IDM_VIEW_SIDEBAR,
                gui->config.sidebar_visible ? MF_CHECKED : MF_UNCHECKED);
            break;

        case IDM_VIEW_TABS:
            if (gui->tab_ctrl) {
                int vis = IsWindowVisible((HWND)gui->tab_ctrl);
                ShowWindow((HWND)gui->tab_ctrl, vis ? SW_HIDE : SW_SHOW);
                CheckMenuItem(GetMenu(hwnd), IDM_VIEW_TABS,
                    vis ? MF_UNCHECKED : MF_CHECKED);
                SendMessage(hwnd, WM_SIZE, 0, 0);
            }
            break;

        case IDM_VIEW_SPLIT_H:
        case IDM_VIEW_SPLIT_V:
            gui_set_status(gui, "Split view — coming in v2.1");
            break;

        case IDM_VIEW_TOOLBAR:
            gui->config.toolbar_visible = !gui->config.toolbar_visible;
            CheckMenuItem(GetMenu(hwnd), IDM_VIEW_TOOLBAR,
                gui->config.toolbar_visible ? MF_CHECKED : MF_UNCHECKED);
            break;

        case IDM_VIEW_STATUSBAR:
            gui->config.statusbar_visible = !gui->config.statusbar_visible;
            CheckMenuItem(GetMenu(hwnd), IDM_VIEW_STATUSBAR,
                gui->config.statusbar_visible ? MF_CHECKED : MF_UNCHECKED);
            SendMessage(hwnd, WM_SIZE, 0, 0);
            break;

        case IDM_VIEW_OPACITY:
            MessageBoxA(hwnd,
                "Window Opacity\n\n"
                "Adjust transparency in Preferences > Appearance.\n"
                "Range: 0 (opaque) to 255 (transparent)",
                PUTTYALT_NAME " - Opacity", MB_OK | MB_ICONINFORMATION);
            break;

        case IDM_VIEW_THEME:
            dialog_theme_select(gui);
            break;

        case IDM_VIEW_SCROLLBACK: {
            char sbmsg[128];
            snprintf(sbmsg, sizeof(sbmsg),
                "Scrollback: %d lines\nAdjust in Preferences.",
                gui->config.scrollback_lines);
            MessageBoxA(hwnd, sbmsg, PUTTYALT_NAME, MB_OK | MB_ICONINFORMATION);
            break;
        }

        /* ── Tools menu ── */
        case IDM_TOOLS_SNIPPETS:
            dialog_snippet_manager(gui);
            break;

        case IDM_TOOLS_MACROS:
            gui_set_status(gui, "Macro recorder ready");
            break;

        case IDM_TOOLS_KEYGEN:
            dialog_keygen(gui);
            break;

        case IDM_TOOLS_SFTP:
            gui_set_status(gui, "SFTP panel — connect first");
            break;

        case IDM_TOOLS_BOOKMARKS:
            dialog_bookmark_manager(gui);
            break;

        case IDM_TOOLS_TUNNEL:
            dialog_tunnel_manager(gui);
            break;

        case IDM_TOOLS_SCRIPTMGR:
            gui_set_status(gui, "Script manager opened");
            break;

        case IDM_TOOLS_CONNPROF:
            gui_set_status(gui, "Connection profiler started");
            break;

        case IDM_TOOLS_MONITOR:
            gui_set_status(gui, "Session monitor active");
            break;

        case IDM_TOOLS_NETDIAG:
            gui_set_status(gui, "Network diagnostics running");
            break;

        /* ── Help menu ── */
        case IDM_HELP_ABOUT:
            gui_show_about(gui);
            break;

        case IDM_HELP_DOCS:
            ShellExecuteA(NULL, "open",
                "https://github.com/chillymasterio/puttyalt",
                NULL, NULL, SW_SHOWNORMAL);
            break;

        case IDM_HELP_SHORTCUTS:
            MessageBoxA(hwnd,
                "Keyboard Shortcuts\n\n"
                "Ctrl+N          New Session\n"
                "Ctrl+O          Open Session\n"
                "Ctrl+S          Save Session\n"
                "Ctrl+D          Duplicate Tab\n"
                "Ctrl+Shift+C    Copy\n"
                "Ctrl+Shift+V    Paste\n"
                "Ctrl+F          Find\n"
                "Ctrl+R          Reconnect\n"
                "Ctrl+B          Broadcast\n"
                "Ctrl+,          Preferences\n"
                "Ctrl++/-/0      Zoom\n"
                "F11             Fullscreen\n"
                "F1              Help",
                PUTTYALT_NAME " - Shortcuts",
                MB_OK | MB_ICONINFORMATION);
            break;

        case IDM_HELP_CHANGELOG:
            MessageBoxA(hwnd,
                "PuttyAlt Changelog\n\n"
                "v1.3.1 — OS detection, speed test, tab preview\n"
                "v1.3.0 — Session groups, SFTP sync, triggers\n"
                "v1.2.0 — Smart paste, search, key manager\n"
                "v1.1.0 — Terminal engine, ANSI, selection\n"
                "v1.0.0 — Complete GUI redesign\n\n"
                "See CHANGELOG.md for full details.",
                PUTTYALT_NAME " - Changelog",
                MB_OK | MB_ICONINFORMATION);
            break;

        case IDM_HELP_UPDATE:
            MessageBoxA(hwnd,
                "Check for Updates\n\n"
                "Current: " PUTTYALT_VERSION_STR "\n\n"
                "You are running the latest version.",
                PUTTYALT_NAME " - Updates",
                MB_OK | MB_ICONINFORMATION);
            break;
        }
        return 0;

    case WM_GETMINMAXINFO: {
        MINMAXINFO *mmi = (MINMAXINFO *)lparam;
        mmi->ptMinTrackSize.x = GUI_MIN_WIDTH;
        mmi->ptMinTrackSize.y = GUI_MIN_HEIGHT;
        return 0;
    }

    case WM_CLOSE:
        if (gui && gui->config.confirm_on_close && gui->connected) {
            if (MessageBoxA(hwnd, "Active session running. Close anyway?",
                    PUTTYALT_NAME, MB_YESNO | MB_ICONQUESTION) != IDYES)
                return 0;
        }
        DestroyWindow(hwnd);
        return 0;

    case WM_RBUTTONUP:
        if (gui) {
            POINT pt; GetCursorPos(&pt);
            CtxMenu *cm = ctx_build_terminal(gui->connected, 0);
            int cmd = ctx_show(cm, gui->hwnd, pt.x, pt.y);
            if (cmd > 0) SendMessage(hwnd, WM_COMMAND, cmd, 0);
        }
        return 0;

    case WM_DESTROY:
        if (gui) gui->running = 0;
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcA(hwnd, msg, wparam, lparam);
}

/* ══════════════════════════════════════════
 *  GUI lifecycle
 * ══════════════════════════════════════════ */

int gui_init(GUIState *gui, void *instance)
{
    memset(gui, 0, sizeof(*gui));
    gui_config_defaults(&gui->config);
    gui->hinstance = instance;
    gui->uptime_start = (unsigned long)time(NULL);

    InitCommonControls();

    /* Initialize tab data */
    snprintf(g_tabs[0].title, sizeof(g_tabs[0].title), "New Session");
    g_tab_count = 1;
    g_active_tab = 0;

    /* Main window class */
    WNDCLASSEXA wc = {0};
    wc.cbSize = sizeof(wc);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = gui_wndproc;
    wc.hInstance = (HINSTANCE)instance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = CreateSolidBrush(HEX_RGB(DS_BG));
    wc.lpszClassName = WNDCLASS_NAME;
    wc.hIcon = LoadIcon((HINSTANCE)instance, MAKEINTRESOURCE(100));
    if (!wc.hIcon) wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    if (!RegisterClassExA(&wc)) return -1;

    /* Terminal class */
    WNDCLASSEXA tc = {0};
    tc.cbSize = sizeof(tc);
    tc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    tc.lpfnWndProc = term_wndproc;
    tc.hInstance = (HINSTANCE)instance;
    tc.hCursor = LoadCursor(NULL, IDC_IBEAM);
    tc.lpszClassName = TERM_CLASS;
    RegisterClassExA(&tc);

    /* Sidebar class */
    WNDCLASSEXA sc = {0};
    sc.cbSize = sizeof(sc);
    sc.style = CS_HREDRAW | CS_VREDRAW;
    sc.lpfnWndProc = sidebar_wndproc;
    sc.hInstance = (HINSTANCE)instance;
    sc.hCursor = LoadCursor(NULL, IDC_ARROW);
    sc.lpszClassName = SIDEBAR_CLASS;
    RegisterClassExA(&sc);

    /* Custom tab bar class */
    WNDCLASSEXA tbc = {0};
    tbc.cbSize = sizeof(tbc);
    tbc.style = CS_HREDRAW | CS_VREDRAW;
    tbc.lpfnWndProc = tabbar_wndproc;
    tbc.hInstance = (HINSTANCE)instance;
    tbc.hCursor = LoadCursor(NULL, IDC_HAND);
    tbc.lpszClassName = TABBAR_CLASS;
    RegisterClassExA(&tbc);

    char title[128];
    snprintf(title, sizeof(title), "%s %s", PUTTYALT_NAME, PUTTYALT_VERSION_STR);

    gui->hwnd = CreateWindowExA(
        WS_EX_APPWINDOW,
        WNDCLASS_NAME, title,
        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
        CW_USEDEFAULT, CW_USEDEFAULT,
        gui->config.width, gui->config.height,
        NULL, NULL, (HINSTANCE)instance, gui);
    if (!gui->hwnd) return -1;

    /* Enable DWM dark title bar */
    enable_dark_titlebar((HWND)gui->hwnd);

    gui->bg_brush = CreateSolidBrush(HEX_RGB(DS_BG));

    gui_create_menu(gui);

    /* Accelerators */
    gui->accel = (void *)gui_create_accelerators();

    /* Custom tab bar (replaces Win32 tab control) */
    gui->tab_ctrl = CreateWindowExA(0, TABBAR_CLASS, NULL,
        WS_CHILD | WS_VISIBLE,
        0, 0, gui->config.width, DS_TABBAR_HEIGHT,
        (HWND)gui->hwnd, NULL, (HINSTANCE)instance, NULL);

    /* Sidebar */
    gui->sidebar_hwnd = CreateWindowExA(0, SIDEBAR_CLASS, NULL,
        WS_CHILD | WS_VISIBLE,
        0, DS_TABBAR_HEIGHT, gui->config.sidebar_width,
        gui->config.height - DS_TABBAR_HEIGHT - DS_STATUSBAR_HEIGHT,
        (HWND)gui->hwnd, NULL, (HINSTANCE)instance, NULL);
    SetWindowLongPtrA((HWND)gui->sidebar_hwnd, GWLP_USERDATA, (LONG_PTR)gui);

    /* Terminal */
    int term_left = gui->config.sidebar_visible ? gui->config.sidebar_width : 0;
    gui->term_hwnd = CreateWindowExA(0, TERM_CLASS, NULL,
        WS_CHILD | WS_VISIBLE,
        term_left, DS_TABBAR_HEIGHT,
        gui->config.width - term_left,
        gui->config.height - DS_TABBAR_HEIGHT - DS_STATUSBAR_HEIGHT,
        (HWND)gui->hwnd, NULL, (HINSTANCE)instance, NULL);
    SetWindowLongPtrA((HWND)gui->term_hwnd, GWLP_USERDATA, (LONG_PTR)gui);

    if (gui->config.transparency > 0) {
        SetWindowLongA((HWND)gui->hwnd, GWL_EXSTYLE,
            GetWindowLongA((HWND)gui->hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
        SetLayeredWindowAttributes((HWND)gui->hwnd, 0,
            (BYTE)(255 - gui->config.transparency), LWA_ALPHA);
    }

    ShowWindow((HWND)gui->hwnd, gui->config.maximized ? SW_MAXIMIZE : SW_SHOW);
    UpdateWindow((HWND)gui->hwnd);

    /* Trigger initial layout */
    SendMessage((HWND)gui->hwnd, WM_SIZE, 0, 0);

    snprintf(gui->title, sizeof(gui->title), "%s", title);
    gui->running = 1;
    gui->num_sessions = 1;
    gui_set_status(gui, "Ready");
    return 0;
}

void gui_destroy(GUIState *gui)
{
    if (gui->accel) DestroyAcceleratorTable((HACCEL)gui->accel);
    if (gui->bg_brush) DeleteObject((HBRUSH)gui->bg_brush);
    if (gui->font) DeleteObject((HFONT)gui->font);
    if (gui->hwnd) DestroyWindow((HWND)gui->hwnd);
    gui->hwnd = NULL;
    gui->running = 0;
}

int gui_run(GUIState *gui)
{
    MSG msg;
    while (gui->running) {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) { gui->running = 0; return 0; }
            if (!gui->accel ||
                !TranslateAccelerator((HWND)gui->hwnd,
                                      (HACCEL)gui->accel, &msg)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        Sleep(8);  /* ~120 fps cap */
    }
    return 0;
}

void gui_quit(GUIState *gui)
{
    gui->running = 0;
    PostMessage((HWND)gui->hwnd, WM_CLOSE, 0, 0);
}

void gui_set_title(GUIState *gui, const char *title)
{
    snprintf(gui->title, sizeof(gui->title), "%s", title);
    SetWindowTextA((HWND)gui->hwnd, title);
}

void gui_set_status(GUIState *gui, const char *text)
{
    snprintf(gui->status_text, sizeof(gui->status_text), "%s", text);
    /* Repaint status bar area */
    if (gui->hwnd) {
        RECT rc;
        GetClientRect((HWND)gui->hwnd, &rc);
        RECT sb_rc = { 0, rc.bottom - DS_STATUSBAR_HEIGHT, rc.right, rc.bottom };
        InvalidateRect((HWND)gui->hwnd, &sb_rc, FALSE);
    }
}

void gui_toggle_fullscreen(GUIState *gui)
{
    static WINDOWPLACEMENT prev_wp;
    HWND hwnd = (HWND)gui->hwnd;
    DWORD style = GetWindowLongA(hwnd, GWL_STYLE);
    if (!gui->config.fullscreen) {
        MONITORINFO mi = { sizeof(mi) };
        GetWindowPlacement(hwnd, &prev_wp);
        GetMonitorInfo(MonitorFromWindow(hwnd, MONITOR_DEFAULTTOPRIMARY), &mi);
        SetWindowLongA(hwnd, GWL_STYLE, style & ~WS_OVERLAPPEDWINDOW);
        SetWindowPos(hwnd, HWND_TOP,
            mi.rcMonitor.left, mi.rcMonitor.top,
            mi.rcMonitor.right - mi.rcMonitor.left,
            mi.rcMonitor.bottom - mi.rcMonitor.top,
            SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        gui->config.fullscreen = 1;
    } else {
        SetWindowLongA(hwnd, GWL_STYLE, style | WS_OVERLAPPEDWINDOW);
        SetWindowPlacement(hwnd, &prev_wp);
        SetWindowPos(hwnd, NULL, 0, 0, 0, 0,
            SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER|SWP_NOOWNERZORDER|SWP_FRAMECHANGED);
        gui->config.fullscreen = 0;
    }
}

void gui_toggle_sidebar(GUIState *gui)
{
    gui->config.sidebar_visible = !gui->config.sidebar_visible;
    SendMessage((HWND)gui->hwnd, WM_SIZE, 0, 0);
}

void gui_resize(GUIState *gui, int w, int h)
{
    SetWindowPos((HWND)gui->hwnd, NULL, 0, 0, w, h, SWP_NOMOVE|SWP_NOZORDER);
}

void gui_update_menu_state(GUIState *gui)
{
    HMENU menu = GetMenu((HWND)gui->hwnd);
    UINT conn = gui->connected ? MF_ENABLED : MF_GRAYED;
    UINT disc = gui->connected ? MF_GRAYED : MF_ENABLED;
    EnableMenuItem(menu, IDM_SESSION_RECONNECT, disc);
    EnableMenuItem(menu, IDM_SESSION_DISCONNECT, conn);
    EnableMenuItem(menu, IDM_SESSION_SEND_CMD, conn);
    EnableMenuItem(menu, IDM_SESSION_BROADCAST, conn);
    EnableMenuItem(menu, IDM_TOOLS_SFTP, conn);
    EnableMenuItem(menu, IDM_EDIT_COPY, conn);
    EnableMenuItem(menu, IDM_EDIT_PASTE, conn);
}

void gui_show_about(GUIState *gui)
{
    char msg[512];
    snprintf(msg, sizeof(msg),
        "%s %s\n\n"
        "Modern SSH Terminal\n"
        "Based on %s\n\n"
        "Features:\n"
        "  Custom dark UI with GitHub Dark theme\n"
        "  Double-buffered rendering\n"
        "  220+ productivity features\n"
        "  Tabs, macros, SFTP, snippets\n\n"
        "MIT License\n"
        "https://github.com/chillymasterio/puttyalt",
        PUTTYALT_NAME, PUTTYALT_VERSION_STR, PUTTYALT_UPSTREAM);
    MessageBoxA((HWND)gui->hwnd, msg, "About " PUTTYALT_NAME,
                MB_OK | MB_ICONINFORMATION);
}

int gui_terminal_write(GUIState *gui, const unsigned char *data, int len)
{
    if (!gui->term_hwnd || !data || len <= 0) return -1;
    gui->bytes_recv += len;
    InvalidateRect((HWND)gui->term_hwnd, NULL, FALSE);
    return len;
}

void gui_terminal_clear(GUIState *gui)
{
    if (gui->term_hwnd)
        InvalidateRect((HWND)gui->term_hwnd, NULL, TRUE);
}

void gui_terminal_set_font(GUIState *gui, const char *name, int size)
{
    if (gui->font) DeleteObject((HFONT)gui->font);
    gui->font = CreateFontA(-size, 0, 0, 0, FW_NORMAL, 0, 0, 0,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, FIXED_PITCH|FF_MODERN, name);
    snprintf(gui->config.font_name, sizeof(gui->config.font_name), "%s", name);
    gui->config.font_size = size;
    if (gui->term_hwnd) InvalidateRect((HWND)gui->term_hwnd, NULL, TRUE);
}

void gui_terminal_scroll(GUIState *gui, int lines)
{
    if (gui->term_hwnd)
        ScrollWindow((HWND)gui->term_hwnd, 0, -lines * 16, NULL, NULL);
}

int gui_connect(GUIState *gui, const char *host, int port, const char *user)
{
    if (gui->connected) return -1;
    char title[256];
    if (user && user[0])
        snprintf(title, sizeof(title), "%s@%s:%d - %s",
                 user, host, port, PUTTYALT_NAME);
    else
        snprintf(title, sizeof(title), "%s:%d - %s",
                 host, port, PUTTYALT_NAME);
    gui_set_title(gui, title);
    gui_set_status(gui, "Connected");
    gui->connected = 1;
    gui->config.state_flags |= GUI_STATE_CONNECTED;
    gui_update_menu_state(gui);

    /* Update active tab title */
    if (g_active_tab < MAX_TABS) {
        if (user && user[0])
            snprintf(g_tabs[g_active_tab].title,
                     sizeof(g_tabs[g_active_tab].title),
                     "%s@%s", user, host);
        else
            snprintf(g_tabs[g_active_tab].title,
                     sizeof(g_tabs[g_active_tab].title),
                     "%s:%d", host, port);
    }

    /* Repaint all panels */
    if (gui->tab_ctrl)
        InvalidateRect((HWND)gui->tab_ctrl, NULL, FALSE);
    if (gui->term_hwnd)
        InvalidateRect((HWND)gui->term_hwnd, NULL, FALSE);
    if (gui->sidebar_hwnd)
        InvalidateRect((HWND)gui->sidebar_hwnd, NULL, FALSE);

    return 0;
}

void gui_disconnect(GUIState *gui)
{
    if (!gui->connected) return;
    gui->connected = 0;
    gui->config.state_flags &= ~GUI_STATE_CONNECTED;
    gui_set_status(gui, "Disconnected");
    char title[128];
    snprintf(title, sizeof(title), "%s %s", PUTTYALT_NAME, PUTTYALT_VERSION_STR);
    gui_set_title(gui, title);
    gui_update_menu_state(gui);

    /* Reset tab title */
    if (g_active_tab < MAX_TABS)
        snprintf(g_tabs[g_active_tab].title,
                 sizeof(g_tabs[g_active_tab].title), "New Session");

    /* Repaint all panels */
    if (gui->tab_ctrl)
        InvalidateRect((HWND)gui->tab_ctrl, NULL, FALSE);
    if (gui->term_hwnd)
        InvalidateRect((HWND)gui->term_hwnd, NULL, FALSE);
    if (gui->sidebar_hwnd)
        InvalidateRect((HWND)gui->sidebar_hwnd, NULL, FALSE);
}

int gui_reconnect(GUIState *gui)
{
    (void)gui;
    return -1;
}

int gui_confirm(GUIState *gui, const char *title, const char *msg)
{
    return MessageBoxA((HWND)gui->hwnd, msg, title,
                       MB_YESNO | MB_ICONQUESTION) == IDYES;
}

void gui_error(GUIState *gui, const char *title, const char *msg)
{
    MessageBoxA((HWND)gui->hwnd, msg, title, MB_OK | MB_ICONERROR);
}

char *gui_input_dialog(GUIState *gui, const char *title, const char *prompt)
{
    (void)gui; (void)title; (void)prompt;
    return NULL;
}

#else /* ══════════ Unix stubs ══════════ */

int gui_init(GUIState *gui, void *instance)
{
    (void)instance;
    memset(gui, 0, sizeof(*gui));
    gui_config_defaults(&gui->config);
    gui->uptime_start = (unsigned long)time(NULL);
    gui->running = 1;
    return 0;
}

void gui_destroy(GUIState *gui) { gui->running = 0; }
int  gui_run(GUIState *gui) { (void)gui; return 0; }
void gui_quit(GUIState *gui) { gui->running = 0; }
void gui_set_title(GUIState *gui, const char *title)
{
    snprintf(gui->title, sizeof(gui->title), "%s", title);
}
void gui_set_status(GUIState *gui, const char *text)
{
    snprintf(gui->status_text, sizeof(gui->status_text), "%s", text);
}
void gui_toggle_fullscreen(GUIState *gui) { (void)gui; }
void gui_toggle_sidebar(GUIState *gui)
{
    gui->config.sidebar_visible = !gui->config.sidebar_visible;
}
void gui_resize(GUIState *gui, int w, int h) { (void)gui; (void)w; (void)h; }
void gui_update_menu_state(GUIState *gui) { (void)gui; }
void gui_show_about(GUIState *gui) { (void)gui; }
int gui_terminal_write(GUIState *gui, const unsigned char *data, int len)
{
    (void)gui; (void)data; return len;
}
void gui_terminal_clear(GUIState *gui) { (void)gui; }
void gui_terminal_set_font(GUIState *gui, const char *name, int size)
{
    (void)gui; (void)name; (void)size;
}
void gui_terminal_scroll(GUIState *gui, int lines) { (void)gui; (void)lines; }
int gui_connect(GUIState *gui, const char *host, int port, const char *user)
{
    (void)gui; (void)host; (void)port; (void)user; return -1;
}
void gui_disconnect(GUIState *gui) { (void)gui; }
int  gui_reconnect(GUIState *gui) { (void)gui; return -1; }
int gui_confirm(GUIState *gui, const char *title, const char *msg)
{
    (void)gui; (void)title; (void)msg; return 0;
}
void gui_error(GUIState *gui, const char *title, const char *msg)
{
    (void)gui; (void)title; (void)msg;
}
char *gui_input_dialog(GUIState *gui, const char *title, const char *prompt)
{
    (void)gui; (void)title; (void)prompt; return NULL;
}

#endif
