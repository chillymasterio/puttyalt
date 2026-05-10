/*
 * puttyalt_gui.c: GUI application framework implementation.
 *
 * Platform-specific window creation, menu setup, and event handling.
 * Win32: native API. Unix: stub for GTK/X11 integration.
 */

#include "puttyalt_gui.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void gui_config_defaults(GUIConfig *cfg)
{
    memset(cfg, 0, sizeof(*cfg));
    cfg->width = GUI_DEFAULT_WIDTH;
    cfg->height = GUI_DEFAULT_HEIGHT;
    cfg->x = -1; /* centered */
    cfg->y = -1;
    cfg->font_size = 10;
    snprintf(cfg->font_name, sizeof(cfg->font_name), "Consolas");
    snprintf(cfg->theme_name, sizeof(cfg->theme_name), "Default");
    cfg->scrollback_lines = 10000;
    cfg->tab_bar_position = 0;
    cfg->toolbar_visible = 1;
    cfg->statusbar_visible = 1;
    cfg->transparency = 0;
    cfg->bell_enabled = 1;
    cfg->confirm_on_close = 1;
}

int gui_config_load(GUIConfig *cfg, const char *path)
{
    FILE *f = fopen(path, "r");
    char line[512];

    if (!f) {
        gui_config_defaults(cfg);
        return -1;
    }

    gui_config_defaults(cfg);

    while (fgets(line, sizeof(line), f)) {
        char *eq = strchr(line, '=');
        if (!eq) continue;
        *eq = '\0';
        char *val = eq + 1;
        /* Strip trailing newline */
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
        else if (strcmp(line, "transparency") == 0) cfg->transparency = atoi(val);
        else if (strcmp(line, "bell") == 0) cfg->bell_enabled = atoi(val);
        else if (strcmp(line, "confirm_close") == 0) cfg->confirm_on_close = atoi(val);
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
    fprintf(f, "transparency=%d\n", cfg->transparency);
    fprintf(f, "bell=%d\n", cfg->bell_enabled);
    fprintf(f, "confirm_close=%d\n", cfg->confirm_on_close);

    fclose(f);
    return 0;
}

#ifdef _WIN32
/*
 * Win32 GUI implementation
 */
#include <windows.h>
#include <commctrl.h>

#pragma comment(lib, "comctl32.lib")

static const char *WNDCLASS_NAME = "PuttyAltWindow";
static const char *TERM_CLASS = "PuttyAltTerminal";

static void gui_create_menu(GUIState *gui)
{
    HMENU menu = CreateMenu();
    HMENU file_menu = CreatePopupMenu();
    HMENU edit_menu = CreatePopupMenu();
    HMENU session_menu = CreatePopupMenu();
    HMENU view_menu = CreatePopupMenu();
    HMENU tools_menu = CreatePopupMenu();
    HMENU help_menu = CreatePopupMenu();

    /* File menu */
    AppendMenuA(file_menu, MF_STRING, IDM_FILE_NEW, "New Session\tCtrl+N");
    AppendMenuA(file_menu, MF_STRING, IDM_FILE_OPEN, "Open Session...\tCtrl+O");
    AppendMenuA(file_menu, MF_STRING, IDM_FILE_SAVE_SESSION, "Save Session\tCtrl+S");
    AppendMenuA(file_menu, MF_STRING, IDM_FILE_DUPLICATE, "Duplicate Tab\tCtrl+D");
    AppendMenuA(file_menu, MF_SEPARATOR, 0, NULL);
    AppendMenuA(file_menu, MF_STRING, IDM_FILE_LOG, "Session Log...");
    AppendMenuA(file_menu, MF_SEPARATOR, 0, NULL);
    AppendMenuA(file_menu, MF_STRING, IDM_FILE_EXIT, "Exit\tAlt+F4");

    /* Edit menu */
    AppendMenuA(edit_menu, MF_STRING, IDM_EDIT_COPY, "Copy\tCtrl+Shift+C");
    AppendMenuA(edit_menu, MF_STRING, IDM_EDIT_PASTE, "Paste\tCtrl+Shift+V");
    AppendMenuA(edit_menu, MF_STRING, IDM_EDIT_SELECTALL, "Select All\tCtrl+Shift+A");
    AppendMenuA(edit_menu, MF_SEPARATOR, 0, NULL);
    AppendMenuA(edit_menu, MF_STRING, IDM_EDIT_FIND, "Find...\tCtrl+F");
    AppendMenuA(edit_menu, MF_STRING, IDM_EDIT_CLEAR, "Clear Scrollback");

    /* Session menu */
    AppendMenuA(session_menu, MF_STRING, IDM_SESSION_RECONNECT, "Reconnect\tCtrl+R");
    AppendMenuA(session_menu, MF_STRING, IDM_SESSION_DISCONNECT, "Disconnect");
    AppendMenuA(session_menu, MF_SEPARATOR, 0, NULL);
    AppendMenuA(session_menu, MF_STRING, IDM_SESSION_SETTINGS, "Settings...");
    AppendMenuA(session_menu, MF_STRING, IDM_SESSION_SEND_CMD, "Send Command...");

    /* View menu */
    AppendMenuA(view_menu, MF_STRING, IDM_VIEW_FULLSCREEN, "Full Screen\tF11");
    AppendMenuA(view_menu, MF_SEPARATOR, 0, NULL);
    AppendMenuA(view_menu, MF_STRING | MF_CHECKED, IDM_VIEW_TABS, "Tab Bar");
    AppendMenuA(view_menu, MF_STRING, IDM_VIEW_SPLIT_H, "Split Horizontal");
    AppendMenuA(view_menu, MF_STRING, IDM_VIEW_SPLIT_V, "Split Vertical");
    AppendMenuA(view_menu, MF_SEPARATOR, 0, NULL);
    AppendMenuA(view_menu, MF_STRING | MF_CHECKED, IDM_VIEW_TOOLBAR, "Toolbar");
    AppendMenuA(view_menu, MF_STRING | MF_CHECKED, IDM_VIEW_STATUSBAR, "Status Bar");
    AppendMenuA(view_menu, MF_SEPARATOR, 0, NULL);
    AppendMenuA(view_menu, MF_STRING, IDM_VIEW_THEME, "Theme...");

    /* Tools menu */
    AppendMenuA(tools_menu, MF_STRING, IDM_TOOLS_SNIPPETS, "Snippets...\tCtrl+Shift+S");
    AppendMenuA(tools_menu, MF_STRING, IDM_TOOLS_MACROS, "Macros...");
    AppendMenuA(tools_menu, MF_STRING, IDM_TOOLS_KEYGEN, "Key Generator...");
    AppendMenuA(tools_menu, MF_SEPARATOR, 0, NULL);
    AppendMenuA(tools_menu, MF_STRING, IDM_TOOLS_SFTP, "SFTP Panel\tCtrl+Shift+F");
    AppendMenuA(tools_menu, MF_STRING, IDM_TOOLS_BROADCAST, "Broadcast Input");
    AppendMenuA(tools_menu, MF_STRING, IDM_TOOLS_BOOKMARKS, "Bookmarks...");
    AppendMenuA(tools_menu, MF_SEPARATOR, 0, NULL);
    AppendMenuA(tools_menu, MF_STRING, IDM_TOOLS_TUNNEL, "Port Tunnels...");
    AppendMenuA(tools_menu, MF_STRING, IDM_TOOLS_NETDIAG, "Network Diagnostics");

    /* Help menu */
    AppendMenuA(help_menu, MF_STRING, IDM_HELP_DOCS, "Documentation");
    AppendMenuA(help_menu, MF_STRING, IDM_HELP_UPDATE, "Check for Updates...");
    AppendMenuA(help_menu, MF_SEPARATOR, 0, NULL);
    AppendMenuA(help_menu, MF_STRING, IDM_HELP_ABOUT, "About PuttyAlt");

    AppendMenuA(menu, MF_POPUP, (UINT_PTR)file_menu, "File");
    AppendMenuA(menu, MF_POPUP, (UINT_PTR)edit_menu, "Edit");
    AppendMenuA(menu, MF_POPUP, (UINT_PTR)session_menu, "Session");
    AppendMenuA(menu, MF_POPUP, (UINT_PTR)view_menu, "View");
    AppendMenuA(menu, MF_POPUP, (UINT_PTR)tools_menu, "Tools");
    AppendMenuA(menu, MF_POPUP, (UINT_PTR)help_menu, "Help");

    SetMenu((HWND)gui->hwnd, menu);
    gui->menu = (void *)menu;
}

static void gui_create_toolbar(GUIState *gui)
{
    HWND tb = CreateWindowExA(0, TOOLBARCLASSNAMEA, NULL,
        WS_CHILD | WS_VISIBLE | TBSTYLE_FLAT | TBSTYLE_TOOLTIPS,
        0, 0, 0, 0,
        (HWND)gui->hwnd, NULL, (HINSTANCE)gui->hinstance, NULL);

    SendMessage(tb, TB_BUTTONSTRUCTSIZE, sizeof(TBBUTTON), 0);

    TBBUTTON buttons[] = {
        {0, IDB_CONNECT, TBSTATE_ENABLED, BTNS_BUTTON, {0}, 0, (INT_PTR)"Connect"},
        {1, IDB_DISCONNECT, TBSTATE_ENABLED, BTNS_BUTTON, {0}, 0, (INT_PTR)"Disconnect"},
        {0, 0, 0, BTNS_SEP, {0}, 0, 0},
        {2, IDB_NEWTAB, TBSTATE_ENABLED, BTNS_BUTTON, {0}, 0, (INT_PTR)"New Tab"},
        {3, IDB_SETTINGS, TBSTATE_ENABLED, BTNS_BUTTON, {0}, 0, (INT_PTR)"Settings"},
        {0, 0, 0, BTNS_SEP, {0}, 0, 0},
        {4, IDB_SFTP, TBSTATE_ENABLED, BTNS_BUTTON, {0}, 0, (INT_PTR)"SFTP"},
        {5, IDB_SNIPPETS, TBSTATE_ENABLED, BTNS_BUTTON, {0}, 0, (INT_PTR)"Snippets"},
    };

    SendMessage(tb, TB_ADDBUTTONSA, sizeof(buttons) / sizeof(buttons[0]),
                (LPARAM)buttons);
    SendMessage(tb, TB_AUTOSIZE, 0, 0);

    gui->toolbar = (void *)tb;
}

static void gui_create_statusbar(GUIState *gui)
{
    HWND sb = CreateWindowExA(0, STATUSCLASSNAMEA, NULL,
        WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP,
        0, 0, 0, 0,
        (HWND)gui->hwnd, NULL, (HINSTANCE)gui->hinstance, NULL);

    int parts[] = { 200, 400, 550, -1 };
    SendMessage(sb, SB_SETPARTS, 4, (LPARAM)parts);
    SendMessage(sb, SB_SETTEXTA, 0, (LPARAM)"Disconnected");
    SendMessage(sb, SB_SETTEXTA, 1, (LPARAM)"No session");
    SendMessage(sb, SB_SETTEXTA, 2, (LPARAM)"0 B / 0 B");
    SendMessage(sb, SB_SETTEXTA, 3, (LPARAM)PUTTYALT_NAME " " PUTTYALT_VERSION_STR);

    gui->statusbar = (void *)sb;
}

static void gui_create_tabctrl(GUIState *gui)
{
    HWND tc = CreateWindowExA(0, WC_TABCONTROLA, NULL,
        WS_CHILD | WS_VISIBLE | TCS_TABS | TCS_FOCUSNEVER,
        0, 0, 0, 30,
        (HWND)gui->hwnd, NULL, (HINSTANCE)gui->hinstance, NULL);

    /* Add default tab */
    TCITEMA item;
    memset(&item, 0, sizeof(item));
    item.mask = TCIF_TEXT;
    item.pszText = "Session 1";
    SendMessage(tc, TCM_INSERTITEMA, 0, (LPARAM)&item);

    gui->tab_ctrl = (void *)tc;
}

static LRESULT CALLBACK gui_wndproc(HWND hwnd, UINT msg,
                                     WPARAM wparam, LPARAM lparam)
{
    GUIState *gui = (GUIState *)GetWindowLongPtrA(hwnd, GWLP_USERDATA);

    switch (msg) {
    case WM_CREATE:
        {
            CREATESTRUCTA *cs = (CREATESTRUCTA *)lparam;
            SetWindowLongPtrA(hwnd, GWLP_USERDATA, (LONG_PTR)cs->lpCreateParams);
        }
        return 0;

    case WM_SIZE:
        if (gui) {
            RECT rc;
            GetClientRect(hwnd, &rc);
            gui->config.width = rc.right - rc.left;
            gui->config.height = rc.bottom - rc.top;

            if (gui->toolbar)
                SendMessage((HWND)gui->toolbar, TB_AUTOSIZE, 0, 0);
            if (gui->statusbar)
                SendMessage((HWND)gui->statusbar, WM_SIZE, 0, 0);
            if (gui->tab_ctrl) {
                int top = gui->toolbar ? 28 : 0;
                MoveWindow((HWND)gui->tab_ctrl, 0, top,
                           rc.right, 28, TRUE);
            }
            if (gui->term_hwnd) {
                int top = (gui->toolbar ? 28 : 0) + 28;
                int bot = gui->statusbar ? 22 : 0;
                MoveWindow((HWND)gui->term_hwnd, 0, top,
                           rc.right, rc.bottom - top - bot, TRUE);
            }
        }
        return 0;

    case WM_COMMAND:
        if (gui) {
            switch (LOWORD(wparam)) {
            case IDM_FILE_EXIT:
                PostMessage(hwnd, WM_CLOSE, 0, 0);
                break;
            case IDM_VIEW_FULLSCREEN:
                gui_toggle_fullscreen(gui);
                break;
            case IDM_HELP_ABOUT:
                gui_show_about(gui);
                break;
            case IDM_FILE_NEW:
                /* TODO: New session dialog */
                break;
            case IDM_SESSION_RECONNECT:
                gui_reconnect(gui);
                break;
            case IDM_SESSION_DISCONNECT:
                gui_disconnect(gui);
                break;
            }
        }
        return 0;

    case WM_GETMINMAXINFO:
        {
            MINMAXINFO *mmi = (MINMAXINFO *)lparam;
            mmi->ptMinTrackSize.x = GUI_MIN_WIDTH;
            mmi->ptMinTrackSize.y = GUI_MIN_HEIGHT;
        }
        return 0;

    case WM_CLOSE:
        if (gui && gui->config.confirm_on_close && gui->connected) {
            if (MessageBoxA(hwnd,
                    "Active session is running. Close anyway?",
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

static LRESULT CALLBACK term_wndproc(HWND hwnd, UINT msg,
                                      WPARAM wparam, LPARAM lparam)
{
    switch (msg) {
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            RECT rc;
            GetClientRect(hwnd, &rc);

            /* Terminal background */
            HBRUSH bg = CreateSolidBrush(RGB(30, 30, 30));
            FillRect(hdc, &rc, bg);
            DeleteObject(bg);

            /* Cursor blink placeholder */
            SetBkMode(hdc, TRANSPARENT);
            SetTextColor(hdc, RGB(204, 204, 204));

            HFONT mono = CreateFontA(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                CLEARTYPE_QUALITY, FIXED_PITCH | FF_MODERN, "Consolas");
            HFONT old = (HFONT)SelectObject(hdc, mono);

            const char *welcome = PUTTYALT_NAME " " PUTTYALT_VERSION_STR
                                  " - Ready to connect";
            TextOutA(hdc, 8, 8, welcome, (int)strlen(welcome));

            SelectObject(hdc, old);
            DeleteObject(mono);

            EndPaint(hwnd, &ps);
        }
        return 0;

    case WM_ERASEBKGND:
        return 1; /* prevent flicker */
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

    /* Register main window class */
    WNDCLASSEXA wc;
    memset(&wc, 0, sizeof(wc));
    wc.cbSize = sizeof(wc);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = gui_wndproc;
    wc.hInstance = (HINSTANCE)instance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = WNDCLASS_NAME;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassExA(&wc))
        return -1;

    /* Register terminal subclass */
    WNDCLASSEXA tc;
    memset(&tc, 0, sizeof(tc));
    tc.cbSize = sizeof(tc);
    tc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    tc.lpfnWndProc = term_wndproc;
    tc.hInstance = (HINSTANCE)instance;
    tc.hCursor = LoadCursor(NULL, IDC_IBEAM);
    tc.lpszClassName = TERM_CLASS;

    RegisterClassExA(&tc);

    /* Create main window */
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

    gui_create_menu(gui);
    gui_create_toolbar(gui);
    gui_create_statusbar(gui);
    gui_create_tabctrl(gui);

    /* Terminal area */
    gui->term_hwnd = CreateWindowExA(
        0, TERM_CLASS, NULL,
        WS_CHILD | WS_VISIBLE,
        0, 56, gui->config.width, gui->config.height - 78,
        (HWND)gui->hwnd, NULL, (HINSTANCE)instance, NULL);

    /* Apply transparency if set */
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
    gui->active_session = 0;

    return 0;
}

void gui_destroy(GUIState *gui)
{
    if (gui->hwnd)
        DestroyWindow((HWND)gui->hwnd);
    gui->hwnd = NULL;
    gui->running = 0;
}

int gui_run(GUIState *gui)
{
    MSG msg;

    while (gui->running) {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                gui->running = 0;
                return 0;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        /* Idle processing: poll IPC, macros, timers etc. */
        Sleep(10);
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
    if (gui->statusbar)
        SendMessage((HWND)gui->statusbar, SB_SETTEXTA, 0, (LPARAM)text);
}

void gui_toggle_fullscreen(GUIState *gui)
{
    static WINDOWPLACEMENT prev_wp;
    HWND hwnd = (HWND)gui->hwnd;
    DWORD style = GetWindowLongA(hwnd, GWL_STYLE);

    if (!gui->config.fullscreen) {
        MONITORINFO mi;
        mi.cbSize = sizeof(mi);
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
            SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
            SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        gui->config.fullscreen = 0;
    }
}

void gui_resize(GUIState *gui, int w, int h)
{
    SetWindowPos((HWND)gui->hwnd, NULL, 0, 0, w, h,
        SWP_NOMOVE | SWP_NOZORDER);
}

void gui_update_menu_state(GUIState *gui)
{
    HMENU menu = GetMenu((HWND)gui->hwnd);
    UINT conn_flag = gui->connected ? MF_ENABLED : MF_GRAYED;
    UINT disc_flag = gui->connected ? MF_GRAYED : MF_ENABLED;

    EnableMenuItem(menu, IDM_SESSION_RECONNECT, disc_flag);
    EnableMenuItem(menu, IDM_SESSION_DISCONNECT, conn_flag);
    EnableMenuItem(menu, IDM_SESSION_SEND_CMD, conn_flag);
    EnableMenuItem(menu, IDM_TOOLS_SFTP, conn_flag);
    EnableMenuItem(menu, IDM_TOOLS_BROADCAST, conn_flag);
}

void gui_show_about(GUIState *gui)
{
    char msg[512];
    snprintf(msg, sizeof(msg),
        "%s version %s\n\n"
        "Enhanced SSH terminal based on %s\n\n"
        "Features: tabs, macros, snippets, SFTP,\n"
        "split-view, themes, key management,\n"
        "session broadcasting, and more.\n\n"
        "https://github.com/chillymasterio/puttyalt",
        PUTTYALT_NAME, PUTTYALT_VERSION_STR, PUTTYALT_UPSTREAM);

    MessageBoxA((HWND)gui->hwnd, msg, "About " PUTTYALT_NAME,
        MB_OK | MB_ICONINFORMATION);
}

int gui_terminal_write(GUIState *gui, const unsigned char *data, int len)
{
    if (!gui->term_hwnd || !data || len <= 0)
        return -1;
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
    if (gui->font)
        DeleteObject((HFONT)gui->font);

    gui->font = CreateFontA(
        -size, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, FIXED_PITCH | FF_MODERN, name);

    snprintf(gui->config.font_name, sizeof(gui->config.font_name), "%s", name);
    gui->config.font_size = size;

    if (gui->term_hwnd)
        InvalidateRect((HWND)gui->term_hwnd, NULL, TRUE);
}

void gui_terminal_scroll(GUIState *gui, int lines)
{
    if (gui->term_hwnd)
        ScrollWindow((HWND)gui->term_hwnd, 0, -lines * 16, NULL, NULL);
}

int gui_connect(GUIState *gui, const char *host, int port, const char *user)
{
    if (gui->connected)
        return -1;

    /* TODO: Establish SSH connection via libputtyalt backend */
    char title[256];
    if (user)
        snprintf(title, sizeof(title), "%s@%s:%d - %s", user, host, port, PUTTYALT_NAME);
    else
        snprintf(title, sizeof(title), "%s:%d - %s", host, port, PUTTYALT_NAME);

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

int gui_reconnect(GUIState *gui)
{
    /* TODO: reconnect using last session parameters */
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
    /* Simplified: real implementation would use a dialog resource */
    (void)gui; (void)title; (void)prompt;
    return NULL;
}

#else /* Unix stub */

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

#endif /* _WIN32 */
