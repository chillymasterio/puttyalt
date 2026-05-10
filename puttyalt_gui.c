/*
 * puttyalt_gui.c: GUI application framework — v1.0.0.
 *
 * Warm blue minimalist theme. Win32 native, Unix stubs.
 */

#include "puttyalt_gui.h"
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
    cfg->font_size = 11;
    snprintf(cfg->font_name, sizeof(cfg->font_name), "Cascadia Code");
    snprintf(cfg->theme_name, sizeof(cfg->theme_name), "Warm Blue");
    cfg->scrollback_lines = 20000;
    cfg->tab_bar_position = 0;
    cfg->toolbar_visible = 1;
    cfg->statusbar_visible = 1;
    cfg->sidebar_visible = 1;
    cfg->sidebar_width = 220;
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
    cfg->color_bg = GUI_COLOR_TERMINAL_BG;
    cfg->color_fg = GUI_COLOR_TERMINAL_FG;
    cfg->color_cursor = GUI_COLOR_CURSOR;
    cfg->color_selection = GUI_COLOR_SELECTION;
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

#pragma comment(lib, "comctl32.lib")

static const char *WNDCLASS_NAME = "PuttyAltWindow";
static const char *TERM_CLASS = "PuttyAltTerminal";
static const char *SIDEBAR_CLASS = "PuttyAltSidebar";

static HBRUSH create_brush(unsigned int hex)
{
    return CreateSolidBrush(HEX_RGB(hex));
}

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
    AppendMenuA(vm, MF_STRING|MF_CHECKED, IDM_VIEW_TOOLBAR, "Toolbar");
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

static void gui_create_statusbar(GUIState *gui)
{
    HWND sb = CreateWindowExA(0, STATUSCLASSNAMEA, NULL,
        WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP,
        0, 0, 0, 0,
        (HWND)gui->hwnd, NULL, (HINSTANCE)gui->hinstance, NULL);
    int parts[] = { 180, 360, 500, 650, -1 };
    SendMessage(sb, SB_SETPARTS, 5, (LPARAM)parts);
    SendMessage(sb, SB_SETTEXTA, 0, (LPARAM)"Ready");
    SendMessage(sb, SB_SETTEXTA, 1, (LPARAM)"No session");
    SendMessage(sb, SB_SETTEXTA, 2, (LPARAM)"0 B \xE2\x86\x91 0 B \xE2\x86\x93");
    SendMessage(sb, SB_SETTEXTA, 3, (LPARAM)"UTF-8");
    SendMessage(sb, SB_SETTEXTA, 4, (LPARAM)PUTTYALT_NAME " " PUTTYALT_VERSION_STR);
    gui->statusbar = (void *)sb;
}

static void gui_create_tabctrl(GUIState *gui)
{
    HWND tc = CreateWindowExA(0, WC_TABCONTROLA, NULL,
        WS_CHILD | WS_VISIBLE | TCS_TABS | TCS_FOCUSNEVER,
        0, 0, 0, 30,
        (HWND)gui->hwnd, NULL, (HINSTANCE)gui->hinstance, NULL);
    TCITEMA item;
    memset(&item, 0, sizeof(item));
    item.mask = TCIF_TEXT;
    item.pszText = "New Session";
    SendMessage(tc, TCM_INSERTITEMA, 0, (LPARAM)&item);
    gui->tab_ctrl = (void *)tc;
}

/* Sidebar: session list / bookmarks / snippets */
static LRESULT CALLBACK sidebar_wndproc(HWND hwnd, UINT msg,
                                         WPARAM wparam, LPARAM lparam)
{
    switch (msg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        RECT rc;
        GetClientRect(hwnd, &rc);

        /* Sidebar background */
        HBRUSH bg = CreateSolidBrush(HEX_RGB(GUI_COLOR_BG_DARK));
        FillRect(hdc, &rc, bg);
        DeleteObject(bg);

        /* Right border line */
        HPEN pen = CreatePen(PS_SOLID, 1, HEX_RGB(GUI_COLOR_BORDER));
        HPEN old_pen = (HPEN)SelectObject(hdc, pen);
        MoveToEx(hdc, rc.right - 1, 0, NULL);
        LineTo(hdc, rc.right - 1, rc.bottom);
        SelectObject(hdc, old_pen);
        DeleteObject(pen);

        /* Header */
        SetBkMode(hdc, TRANSPARENT);
        HFONT hf = CreateFontA(14, 0, 0, 0, FW_BOLD, 0, 0, 0,
            DEFAULT_CHARSET, 0, 0, CLEARTYPE_QUALITY,
            DEFAULT_PITCH, "Segoe UI");
        HFONT old_f = (HFONT)SelectObject(hdc, hf);
        SetTextColor(hdc, HEX_RGB(GUI_COLOR_TEXT));

        RECT hdr = { 16, 12, rc.right - 8, 36 };
        DrawTextA(hdc, "Sessions", -1, &hdr, DT_LEFT | DT_SINGLELINE);
        SelectObject(hdc, old_f);
        DeleteObject(hf);

        /* Separator line */
        HPEN sep = CreatePen(PS_SOLID, 1, HEX_RGB(GUI_COLOR_BORDER));
        HPEN old2 = (HPEN)SelectObject(hdc, sep);
        MoveToEx(hdc, 12, 42, NULL);
        LineTo(hdc, rc.right - 12, 42);
        SelectObject(hdc, old2);
        DeleteObject(sep);

        /* Placeholder items */
        hf = CreateFontA(13, 0, 0, 0, FW_NORMAL, 0, 0, 0,
            DEFAULT_CHARSET, 0, 0, CLEARTYPE_QUALITY,
            DEFAULT_PITCH, "Segoe UI");
        SelectObject(hdc, hf);
        SetTextColor(hdc, HEX_RGB(GUI_COLOR_TEXT_DIM));

        RECT item_rc = { 16, 54, rc.right - 8, 74 };
        DrawTextA(hdc, "No saved sessions", -1, &item_rc,
                  DT_LEFT | DT_SINGLELINE | DT_VCENTER);
        SelectObject(hdc, old_f);
        DeleteObject(hf);

        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_ERASEBKGND:
        return 1;
    }
    return DefWindowProcA(hwnd, msg, wparam, lparam);
}

static LRESULT CALLBACK term_wndproc(HWND hwnd, UINT msg,
                                      WPARAM wparam, LPARAM lparam)
{
    switch (msg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        RECT rc;
        GetClientRect(hwnd, &rc);

        HBRUSH bg = CreateSolidBrush(HEX_RGB(GUI_COLOR_TERMINAL_BG));
        FillRect(hdc, &rc, bg);
        DeleteObject(bg);

        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, HEX_RGB(GUI_COLOR_TERMINAL_FG));

        HFONT mono = CreateFontA(15, 0, 0, 0, FW_NORMAL, 0, 0, 0,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY, FIXED_PITCH | FF_MODERN, "Cascadia Code");
        HFONT old = (HFONT)SelectObject(hdc, mono);

        /* Welcome screen */
        int cy = 20;
        SetTextColor(hdc, HEX_RGB(GUI_COLOR_PRIMARY));
        const char *banner = PUTTYALT_NAME " " PUTTYALT_VERSION_STR;
        TextOutA(hdc, 20, cy, banner, (int)strlen(banner));
        cy += 22;

        SetTextColor(hdc, HEX_RGB(GUI_COLOR_TEXT_DIM));
        const char *sub = "Enhanced SSH Terminal — Ready to connect";
        TextOutA(hdc, 20, cy, sub, (int)strlen(sub));
        cy += 30;

        SetTextColor(hdc, HEX_RGB(GUI_COLOR_ACCENT));
        const char *tip1 = "Ctrl+N  New session";
        const char *tip2 = "Ctrl+O  Open saved session";
        const char *tip3 = "Ctrl+,  Preferences";
        TextOutA(hdc, 20, cy, tip1, (int)strlen(tip1)); cy += 20;
        TextOutA(hdc, 20, cy, tip2, (int)strlen(tip2)); cy += 20;
        TextOutA(hdc, 20, cy, tip3, (int)strlen(tip3));

        SelectObject(hdc, old);
        DeleteObject(mono);
        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_ERASEBKGND:
        return 1;
    }
    return DefWindowProcA(hwnd, msg, wparam, lparam);
}

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

            if (gui->statusbar)
                SendMessage((HWND)gui->statusbar, WM_SIZE, 0, 0);

            int top = 0;
            int sb_left = 0;
            int sb_h = gui->statusbar ? 22 : 0;

            /* Tab bar */
            if (gui->tab_ctrl) {
                MoveWindow((HWND)gui->tab_ctrl, 0, top, rc.right, 28, TRUE);
                top += 28;
            }

            /* Sidebar */
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
        }
        return 0;

    case WM_COMMAND:
        if (gui) {
            switch (LOWORD(wparam)) {
            case IDM_FILE_EXIT: PostMessage(hwnd, WM_CLOSE, 0, 0); break;
            case IDM_VIEW_FULLSCREEN: gui_toggle_fullscreen(gui); break;
            case IDM_VIEW_SIDEBAR: gui_toggle_sidebar(gui); break;
            case IDM_VIEW_ZOOM_IN: gui_zoom_in(gui); break;
            case IDM_VIEW_ZOOM_OUT: gui_zoom_out(gui); break;
            case IDM_VIEW_ZOOM_RESET: gui_zoom_reset(gui); break;
            case IDM_HELP_ABOUT: gui_show_about(gui); break;
            case IDM_SESSION_RECONNECT: gui_reconnect(gui); break;
            case IDM_SESSION_DISCONNECT: gui_disconnect(gui); break;
            }
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

    case WM_DESTROY:
        if (gui) gui->running = 0;
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcA(hwnd, msg, wparam, lparam);
}

int gui_init(GUIState *gui, void *instance)
{
    memset(gui, 0, sizeof(*gui));
    gui_config_defaults(&gui->config);
    gui->hinstance = instance;
    gui->uptime_start = (unsigned long)time(NULL);

    InitCommonControls();

    /* Main window class */
    WNDCLASSEXA wc = {0};
    wc.cbSize = sizeof(wc);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = gui_wndproc;
    wc.hInstance = (HINSTANCE)instance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = CreateSolidBrush(HEX_RGB(GUI_COLOR_BG));
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

    gui->bg_brush = CreateSolidBrush(HEX_RGB(GUI_COLOR_BG));

    gui_create_menu(gui);
    gui_create_statusbar(gui);
    gui_create_tabctrl(gui);

    /* Sidebar */
    gui->sidebar_hwnd = CreateWindowExA(0, SIDEBAR_CLASS, NULL,
        WS_CHILD | WS_VISIBLE,
        0, 28, gui->config.sidebar_width, gui->config.height - 50,
        (HWND)gui->hwnd, NULL, (HINSTANCE)instance, NULL);

    /* Terminal */
    int term_left = gui->config.sidebar_visible ? gui->config.sidebar_width : 0;
    gui->term_hwnd = CreateWindowExA(0, TERM_CLASS, NULL,
        WS_CHILD | WS_VISIBLE,
        term_left, 28, gui->config.width - term_left, gui->config.height - 50,
        (HWND)gui->hwnd, NULL, (HINSTANCE)instance, NULL);

    if (gui->config.transparency > 0) {
        SetWindowLongA((HWND)gui->hwnd, GWL_EXSTYLE,
            GetWindowLongA((HWND)gui->hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
        SetLayeredWindowAttributes((HWND)gui->hwnd, 0,
            (BYTE)(255 - gui->config.transparency), LWA_ALPHA);
    }

    ShowWindow((HWND)gui->hwnd, gui->config.maximized ? SW_MAXIMIZE : SW_SHOW);
    UpdateWindow((HWND)gui->hwnd);

    snprintf(gui->title, sizeof(gui->title), "%s", title);
    gui->running = 1;
    gui->num_sessions = 1;
    return 0;
}

void gui_destroy(GUIState *gui)
{
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
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        Sleep(10);
    }
    return 0;
}

void gui_quit(GUIState *gui) { gui->running = 0; PostMessage((HWND)gui->hwnd, WM_CLOSE, 0, 0); }

void gui_set_title(GUIState *gui, const char *title)
{
    snprintf(gui->title, sizeof(gui->title), "%s", title);
    SetWindowTextA((HWND)gui->hwnd, title);
}

void gui_set_status(GUIState *gui, const char *text)
{
    snprintf(gui->status_text, sizeof(gui->status_text), "%s", text);
    if (gui->statusbar) SendMessage((HWND)gui->statusbar, SB_SETTEXTA, 0, (LPARAM)text);
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
}

void gui_show_about(GUIState *gui)
{
    char msg[512];
    snprintf(msg, sizeof(msg),
        "%s version %s\n\n"
        "Enhanced SSH terminal based on %s\n\n"
        "Warm Blue theme with minimalist design.\n"
        "Tabs, macros, SFTP, split-view, themes,\n"
        "plugins, and 100+ features.\n\n"
        "MIT License\n"
        "https://github.com/chillymasterio/puttyalt",
        PUTTYALT_NAME, PUTTYALT_VERSION_STR, PUTTYALT_UPSTREAM);
    MessageBoxA((HWND)gui->hwnd, msg, "About " PUTTYALT_NAME, MB_OK|MB_ICONINFORMATION);
}

int gui_terminal_write(GUIState *gui, const unsigned char *data, int len)
{
    if (!gui->term_hwnd || !data || len <= 0) return -1;
    gui->bytes_recv += len;
    InvalidateRect((HWND)gui->term_hwnd, NULL, FALSE);
    return len;
}

void gui_terminal_clear(GUIState *gui) { if (gui->term_hwnd) InvalidateRect((HWND)gui->term_hwnd, NULL, TRUE); }

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
    if (gui->term_hwnd) ScrollWindow((HWND)gui->term_hwnd, 0, -lines*16, NULL, NULL);
}

int gui_connect(GUIState *gui, const char *host, int port, const char *user)
{
    if (gui->connected) return -1;
    char title[256];
    if (user) snprintf(title, sizeof(title), "%s@%s:%d - %s", user, host, port, PUTTYALT_NAME);
    else snprintf(title, sizeof(title), "%s:%d - %s", host, port, PUTTYALT_NAME);
    gui_set_title(gui, title);
    gui_set_status(gui, "Connected");
    gui->connected = 1;
    gui->config.state_flags |= GUI_STATE_CONNECTED;
    gui_update_menu_state(gui);
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
}

int gui_reconnect(GUIState *gui) { (void)gui; return -1; }

int gui_confirm(GUIState *gui, const char *title, const char *msg)
{
    return MessageBoxA((HWND)gui->hwnd, msg, title, MB_YESNO|MB_ICONQUESTION) == IDYES;
}

void gui_error(GUIState *gui, const char *title, const char *msg)
{
    MessageBoxA((HWND)gui->hwnd, msg, title, MB_OK|MB_ICONERROR);
}

char *gui_input_dialog(GUIState *gui, const char *title, const char *prompt)
{
    (void)gui; (void)title; (void)prompt; return NULL;
}

#else /* Unix stubs */

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
void gui_set_title(GUIState *gui, const char *title) { snprintf(gui->title, sizeof(gui->title), "%s", title); }
void gui_set_status(GUIState *gui, const char *text) { snprintf(gui->status_text, sizeof(gui->status_text), "%s", text); }
void gui_toggle_fullscreen(GUIState *gui) { (void)gui; }
void gui_toggle_sidebar(GUIState *gui) { gui->config.sidebar_visible = !gui->config.sidebar_visible; }
void gui_resize(GUIState *gui, int w, int h) { (void)gui; (void)w; (void)h; }
void gui_update_menu_state(GUIState *gui) { (void)gui; }
void gui_show_about(GUIState *gui) { (void)gui; }
int gui_terminal_write(GUIState *gui, const unsigned char *data, int len) { (void)gui; (void)data; return len; }
void gui_terminal_clear(GUIState *gui) { (void)gui; }
void gui_terminal_set_font(GUIState *gui, const char *name, int size) { (void)gui; (void)name; (void)size; }
void gui_terminal_scroll(GUIState *gui, int lines) { (void)gui; (void)lines; }
int gui_connect(GUIState *gui, const char *host, int port, const char *user) { (void)gui; (void)host; (void)port; (void)user; return -1; }
void gui_disconnect(GUIState *gui) { (void)gui; }
int  gui_reconnect(GUIState *gui) { (void)gui; return -1; }
int gui_confirm(GUIState *gui, const char *title, const char *msg) { (void)gui; (void)title; (void)msg; return 0; }
void gui_error(GUIState *gui, const char *title, const char *msg) { (void)gui; (void)title; (void)msg; }
char *gui_input_dialog(GUIState *gui, const char *title, const char *prompt) { (void)gui; (void)title; (void)prompt; return NULL; }

#endif
