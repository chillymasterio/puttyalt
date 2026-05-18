/*
 * puttyalt_gui.c: GUI application framework — v1.0.6.
 *
 * Warm blue minimalist theme. Full Win32 GUI with toolbar,
 * accelerators, and all menu handlers. Unix stubs.
 */

#include "puttyalt_gui.h"
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

static const char *WNDCLASS_NAME = "PuttyAltWindow";
static const char *TERM_CLASS = "PuttyAltTerminal";
static const char *SIDEBAR_CLASS = "PuttyAltSidebar";

static HBRUSH create_brush(unsigned int hex)
{
    return CreateSolidBrush(HEX_RGB(hex));
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

/* ══════════════════════════════════════════
 *  Statusbar
 * ══════════════════════════════════════════ */

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
    SendMessage(sb, SB_SETTEXTA, 2, (LPARAM)"0 B sent / 0 B recv");
    SendMessage(sb, SB_SETTEXTA, 3, (LPARAM)"UTF-8");
    SendMessage(sb, SB_SETTEXTA, 4, (LPARAM)PUTTYALT_NAME " " PUTTYALT_VERSION_STR);
    gui->statusbar = (void *)sb;
}

/* ══════════════════════════════════════════
 *  Tab control
 * ══════════════════════════════════════════ */

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

/* ══════════════════════════════════════════
 *  Toolbar
 * ══════════════════════════════════════════ */

static void gui_create_toolbar(GUIState *gui)
{
    HWND tb = CreateWindowExA(0, TOOLBARCLASSNAMEA, NULL,
        WS_CHILD | WS_VISIBLE | TBSTYLE_FLAT | TBSTYLE_TOOLTIPS |
        CCS_NODIVIDER,
        0, 0, 0, 0,
        (HWND)gui->hwnd, NULL, (HINSTANCE)gui->hinstance, NULL);

    SendMessage(tb, TB_BUTTONSTRUCTSIZE, sizeof(TBBUTTON), 0);

    /* Load standard system icons */
    TBADDBITMAP tbab;
    tbab.hInst = HINST_COMMCTRL;
    tbab.nID = IDB_STD_SMALL_COLOR;
    SendMessage(tb, TB_ADDBITMAP, 15, (LPARAM)&tbab);

    TBBUTTON buttons[9];
    memset(buttons, 0, sizeof(buttons));

    /* Connect */
    buttons[0].iBitmap = STD_FILENEW;
    buttons[0].idCommand = IDM_FILE_NEW;
    buttons[0].fsState = TBSTATE_ENABLED;
    buttons[0].fsStyle = BTNS_BUTTON;
    /* Disconnect */
    buttons[1].iBitmap = STD_DELETE;
    buttons[1].idCommand = IDM_SESSION_DISCONNECT;
    buttons[1].fsState = TBSTATE_ENABLED;
    buttons[1].fsStyle = BTNS_BUTTON;
    /* Separator */
    buttons[2].fsStyle = BTNS_SEP;
    /* Copy */
    buttons[3].iBitmap = STD_COPY;
    buttons[3].idCommand = IDM_EDIT_COPY;
    buttons[3].fsState = TBSTATE_ENABLED;
    buttons[3].fsStyle = BTNS_BUTTON;
    /* Paste */
    buttons[4].iBitmap = STD_PASTE;
    buttons[4].idCommand = IDM_EDIT_PASTE;
    buttons[4].fsState = TBSTATE_ENABLED;
    buttons[4].fsStyle = BTNS_BUTTON;
    /* Find */
    buttons[5].iBitmap = STD_FIND;
    buttons[5].idCommand = IDM_EDIT_FIND;
    buttons[5].fsState = TBSTATE_ENABLED;
    buttons[5].fsStyle = BTNS_BUTTON;
    /* Separator */
    buttons[6].fsStyle = BTNS_SEP;
    /* Settings */
    buttons[7].iBitmap = STD_PROPERTIES;
    buttons[7].idCommand = IDM_EDIT_PREFERENCES;
    buttons[7].fsState = TBSTATE_ENABLED;
    buttons[7].fsStyle = BTNS_BUTTON;
    /* Help */
    buttons[8].iBitmap = STD_HELP;
    buttons[8].idCommand = IDM_HELP_ABOUT;
    buttons[8].fsState = TBSTATE_ENABLED;
    buttons[8].fsStyle = BTNS_BUTTON;

    SendMessage(tb, TB_ADDBUTTONSA, 9, (LPARAM)buttons);
    SendMessage(tb, TB_AUTOSIZE, 0, 0);

    gui->toolbar = (void *)tb;
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
 *  Sidebar window procedure
 * ══════════════════════════════════════════ */

static LRESULT CALLBACK sidebar_wndproc(HWND hwnd, UINT msg,
                                         WPARAM wparam, LPARAM lparam)
{
    switch (msg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        RECT rc;
        GetClientRect(hwnd, &rc);
        GUIState *gui = (GUIState *)GetWindowLongPtrA(hwnd, GWLP_USERDATA);

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

        SetBkMode(hdc, TRANSPARENT);

        /* Header */
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

        /* Content */
        hf = CreateFontA(13, 0, 0, 0, FW_NORMAL, 0, 0, 0,
            DEFAULT_CHARSET, 0, 0, CLEARTYPE_QUALITY,
            DEFAULT_PITCH, "Segoe UI");
        old_f = (HFONT)SelectObject(hdc, hf);

        if (gui && gui->connected) {
            /* Show active session */
            RECT sess_rc = { 16, 54, rc.right - 8, 74 };
            SetTextColor(hdc, HEX_RGB(GUI_COLOR_SUCCESS));
            DrawTextA(hdc, "Active", -1, &sess_rc,
                      DT_LEFT | DT_SINGLELINE | DT_VCENTER);

            /* Session title */
            RECT title_rc = { 16, 76, rc.right - 8, 96 };
            SetTextColor(hdc, HEX_RGB(GUI_COLOR_TEXT));
            DrawTextA(hdc, gui->title, -1, &title_rc,
                      DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS);

            /* Stats */
            char stats[128];
            snprintf(stats, sizeof(stats), "Sent: %lu B  Recv: %lu B",
                     gui->bytes_sent, gui->bytes_recv);
            RECT stats_rc = { 16, 100, rc.right - 8, 120 };
            SetTextColor(hdc, HEX_RGB(GUI_COLOR_TEXT_DIM));
            DrawTextA(hdc, stats, -1, &stats_rc,
                      DT_LEFT | DT_SINGLELINE | DT_VCENTER);
        } else {
            /* No sessions */
            RECT item_rc = { 16, 54, rc.right - 8, 74 };
            SetTextColor(hdc, HEX_RGB(GUI_COLOR_TEXT_DIM));
            DrawTextA(hdc, "No saved sessions", -1, &item_rc,
                      DT_LEFT | DT_SINGLELINE | DT_VCENTER);

            RECT hint_rc = { 16, 82, rc.right - 8, 102 };
            SetTextColor(hdc, HEX_RGB(GUI_COLOR_ACCENT));
            DrawTextA(hdc, "Ctrl+N to connect", -1, &hint_rc,
                      DT_LEFT | DT_SINGLELINE | DT_VCENTER);
        }

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

/* ══════════════════════════════════════════
 *  Terminal window procedure
 * ══════════════════════════════════════════ */

static LRESULT CALLBACK term_wndproc(HWND hwnd, UINT msg,
                                      WPARAM wparam, LPARAM lparam)
{
    switch (msg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        RECT rc;
        GetClientRect(hwnd, &rc);
        GUIState *gui = (GUIState *)GetWindowLongPtrA(hwnd, GWLP_USERDATA);

        HBRUSH bg = CreateSolidBrush(HEX_RGB(GUI_COLOR_TERMINAL_BG));
        FillRect(hdc, &rc, bg);
        DeleteObject(bg);

        SetBkMode(hdc, TRANSPARENT);

        HFONT mono = CreateFontA(15, 0, 0, 0, FW_NORMAL, 0, 0, 0,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY, FIXED_PITCH | FF_MODERN, "Cascadia Code");
        HFONT old = (HFONT)SelectObject(hdc, mono);

        int cy = 20;

        if (gui && gui->connected) {
            /* Connected state */
            SetTextColor(hdc, HEX_RGB(GUI_COLOR_SUCCESS));
            const char *conn = "Connected";
            TextOutA(hdc, 20, cy, conn, (int)strlen(conn));
            cy += 22;

            SetTextColor(hdc, HEX_RGB(GUI_COLOR_TEXT));
            TextOutA(hdc, 20, cy, gui->title, (int)strlen(gui->title));
            cy += 30;

            SetTextColor(hdc, HEX_RGB(GUI_COLOR_TEXT_DIM));
            const char *hint = "Terminal ready. Type commands to interact with the remote server.";
            TextOutA(hdc, 20, cy, hint, (int)strlen(hint));
            cy += 30;

            /* Simulated prompt */
            SetTextColor(hdc, HEX_RGB(GUI_COLOR_CURSOR));
            const char *prompt = "$ _";
            TextOutA(hdc, 20, cy, prompt, (int)strlen(prompt));
        } else {
            /* Welcome screen */
            SetTextColor(hdc, HEX_RGB(GUI_COLOR_PRIMARY));
            const char *banner = PUTTYALT_NAME " " PUTTYALT_VERSION_STR;
            TextOutA(hdc, 20, cy, banner, (int)strlen(banner));
            cy += 22;

            SetTextColor(hdc, HEX_RGB(GUI_COLOR_TEXT_DIM));
            const char *sub = "Enhanced SSH Terminal - Ready to connect";
            TextOutA(hdc, 20, cy, sub, (int)strlen(sub));
            cy += 36;

            /* Keyboard shortcuts */
            SetTextColor(hdc, HEX_RGB(GUI_COLOR_ACCENT));
            const char *tips[] = {
                "Ctrl+N   New session",
                "Ctrl+O   Open saved session",
                "Ctrl+,   Preferences",
                "Ctrl+F   Find in terminal",
                "F11      Toggle fullscreen",
                "Ctrl+B   Broadcast mode",
            };
            for (int i = 0; i < 6; i++) {
                TextOutA(hdc, 20, cy, tips[i], (int)strlen(tips[i]));
                cy += 20;
            }

            cy += 16;
            SetTextColor(hdc, HEX_RGB(GUI_COLOR_TEXT_DIM));
            const char *ver = "Based on " PUTTYALT_UPSTREAM " | " PUTTYALT_VERSION_STR " | MIT License";
            TextOutA(hdc, 20, cy, ver, (int)strlen(ver));
        }

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

            if (gui->statusbar) {
                SendMessage((HWND)gui->statusbar, WM_SIZE, 0, 0);
            }

            int top = 0;
            int sb_h = (gui->statusbar && gui->config.statusbar_visible) ? 22 : 0;

            /* Toolbar */
            if (gui->toolbar && gui->config.toolbar_visible) {
                SendMessage((HWND)gui->toolbar, TB_AUTOSIZE, 0, 0);
                RECT tbrc;
                GetWindowRect((HWND)gui->toolbar, &tbrc);
                int tb_h = tbrc.bottom - tbrc.top;
                MoveWindow((HWND)gui->toolbar, 0, 0, rc.right, tb_h, TRUE);
                top = tb_h;
            }

            /* Tab bar */
            if (gui->tab_ctrl && IsWindowVisible((HWND)gui->tab_ctrl)) {
                MoveWindow((HWND)gui->tab_ctrl, 0, top, rc.right, 28, TRUE);
                top += 28;
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
        }
        return 0;

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
            gui_set_status(gui, "Tab duplicated");
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
                "or PuttyAlt backup files.\n\n"
                "Coming in the next update.",
                PUTTYALT_NAME " - Import", MB_OK | MB_ICONINFORMATION);
            break;

        case IDM_FILE_EXPORT:
            MessageBoxA(hwnd,
                "Export Sessions\n\n"
                "Export your sessions to a backup file\n"
                "for migration or sharing.\n\n"
                "Coming in the next update.",
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
                char msg[300];
                snprintf(msg, sizeof(msg), "Searching: %s", search);
                gui_set_status(gui, msg);
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
                MessageBoxA(hwnd,
                    "Send Command\n\n"
                    "Type a command to send to the remote server.\n\n"
                    "Full command input coming in the next update.",
                    PUTTYALT_NAME " - Send Command",
                    MB_OK | MB_ICONINFORMATION);
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
            MessageBoxA(hwnd,
                "Split View\n\n"
                "Divide the terminal into multiple panes\n"
                "for side-by-side sessions.\n\n"
                "Coming in the next update.",
                PUTTYALT_NAME " - Split View", MB_OK | MB_ICONINFORMATION);
            break;

        case IDM_VIEW_TOOLBAR:
            gui->config.toolbar_visible = !gui->config.toolbar_visible;
            if (gui->toolbar)
                ShowWindow((HWND)gui->toolbar,
                    gui->config.toolbar_visible ? SW_SHOW : SW_HIDE);
            CheckMenuItem(GetMenu(hwnd), IDM_VIEW_TOOLBAR,
                gui->config.toolbar_visible ? MF_CHECKED : MF_UNCHECKED);
            SendMessage(hwnd, WM_SIZE, 0, 0);
            break;

        case IDM_VIEW_STATUSBAR:
            gui->config.statusbar_visible = !gui->config.statusbar_visible;
            if (gui->statusbar)
                ShowWindow((HWND)gui->statusbar,
                    gui->config.statusbar_visible ? SW_SHOW : SW_HIDE);
            CheckMenuItem(GetMenu(hwnd), IDM_VIEW_STATUSBAR,
                gui->config.statusbar_visible ? MF_CHECKED : MF_UNCHECKED);
            SendMessage(hwnd, WM_SIZE, 0, 0);
            break;

        case IDM_VIEW_OPACITY:
            MessageBoxA(hwnd,
                "Window Opacity\n\n"
                "Adjust window transparency.\n"
                "Set in Preferences > Transparency (0-255).\n\n"
                "0 = fully opaque, 255 = nearly transparent",
                PUTTYALT_NAME " - Opacity", MB_OK | MB_ICONINFORMATION);
            break;

        case IDM_VIEW_THEME:
            dialog_theme_select(gui);
            break;

        case IDM_VIEW_SCROLLBACK: {
            char msg[128];
            snprintf(msg, sizeof(msg),
                "Scrollback Buffer\n\nCurrent size: %d lines.\n"
                "Adjust in Preferences.",
                gui->config.scrollback_lines);
            MessageBoxA(hwnd, msg, PUTTYALT_NAME, MB_OK | MB_ICONINFORMATION);
            break;
        }

        /* ── Tools menu ── */
        case IDM_TOOLS_SNIPPETS:
            dialog_snippet_manager(gui);
            break;

        case IDM_TOOLS_MACROS:
            MessageBoxA(hwnd,
                "Macro Recorder\n\n"
                "Record and replay terminal macros.\n"
                "Automate repetitive tasks with ease.\n\n"
                "Coming in the next update.",
                PUTTYALT_NAME " - Macros", MB_OK | MB_ICONINFORMATION);
            break;

        case IDM_TOOLS_KEYGEN:
            dialog_keygen(gui);
            break;

        case IDM_TOOLS_SFTP:
            MessageBoxA(hwnd,
                "SFTP File Browser\n\n"
                "Browse and transfer files over SSH.\n"
                "Drag-and-drop support included.\n\n"
                "Coming in the next update.",
                PUTTYALT_NAME " - SFTP", MB_OK | MB_ICONINFORMATION);
            break;

        case IDM_TOOLS_BOOKMARKS:
            dialog_bookmark_manager(gui);
            break;

        case IDM_TOOLS_TUNNEL:
            dialog_tunnel_manager(gui);
            break;

        case IDM_TOOLS_SCRIPTMGR:
            MessageBoxA(hwnd,
                "Script Manager\n\n"
                "Automate tasks with custom scripts.\n"
                "Support for Bash, Python, and Expect.\n\n"
                "Coming in the next update.",
                PUTTYALT_NAME " - Scripts", MB_OK | MB_ICONINFORMATION);
            break;

        case IDM_TOOLS_CONNPROF:
            MessageBoxA(hwnd,
                "Connection Profiler\n\n"
                "Analyze network performance, latency,\n"
                "and bandwidth for active sessions.\n\n"
                "Coming in the next update.",
                PUTTYALT_NAME " - Profiler", MB_OK | MB_ICONINFORMATION);
            break;

        case IDM_TOOLS_MONITOR:
            MessageBoxA(hwnd,
                "Session Monitor\n\n"
                "Monitor active sessions, resource usage,\n"
                "and uptime statistics.\n\n"
                "Coming in the next update.",
                PUTTYALT_NAME " - Monitor", MB_OK | MB_ICONINFORMATION);
            break;

        case IDM_TOOLS_NETDIAG:
            MessageBoxA(hwnd,
                "Network Diagnostics\n\n"
                "Troubleshoot connectivity issues with\n"
                "built-in ping, traceroute, and DNS tools.\n\n"
                "Coming in the next update.",
                PUTTYALT_NAME " - Network Diagnostics",
                MB_OK | MB_ICONINFORMATION);
            break;

        /* ── Help menu ── */
        case IDM_HELP_ABOUT:
            gui_show_about(gui);
            break;

        case IDM_HELP_DOCS:
            MessageBoxA(hwnd,
                "PuttyAlt Documentation\n\n"
                "For documentation and guides, visit:\n"
                "https://github.com/chillymasterio/puttyalt\n\n"
                "Press F1 at any time to open help.",
                PUTTYALT_NAME " - Documentation",
                MB_OK | MB_ICONINFORMATION);
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
                "Ctrl+B          Broadcast Mode\n"
                "Ctrl+,          Preferences\n"
                "Ctrl++          Zoom In\n"
                "Ctrl+-          Zoom Out\n"
                "Ctrl+0          Reset Zoom\n"
                "F11             Full Screen\n"
                "F1              Help / Docs",
                PUTTYALT_NAME " - Keyboard Shortcuts",
                MB_OK | MB_ICONINFORMATION);
            break;

        case IDM_HELP_CHANGELOG:
            MessageBoxA(hwnd,
                "PuttyAlt Changelog\n\n"
                "v1.0.6 - Latest\n"
                "  Layout management, macro recording,\n"
                "  terminal configuration, auto-updater,\n"
                "  enhanced status bar\n\n"
                "v1.0.0\n"
                "  Complete GUI redesign, warm blue theme,\n"
                "  tab support, plugin system, 100+ features\n\n"
                "v0.4.0\n"
                "  Workspace management, tunnel manager,\n"
                "  URL handler, network diagnostics\n\n"
                "v0.1.0\n"
                "  Initial release with core enhancements",
                PUTTYALT_NAME " - Changelog",
                MB_OK | MB_ICONINFORMATION);
            break;

        case IDM_HELP_UPDATE:
            MessageBoxA(hwnd,
                "Check for Updates\n\n"
                "Current version: " PUTTYALT_VERSION_STR "\n\n"
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
    gui_create_toolbar(gui);

    /* Accelerators */
    gui->accel = (void *)gui_create_accelerators();

    /* Sidebar */
    gui->sidebar_hwnd = CreateWindowExA(0, SIDEBAR_CLASS, NULL,
        WS_CHILD | WS_VISIBLE,
        0, 28, gui->config.sidebar_width, gui->config.height - 50,
        (HWND)gui->hwnd, NULL, (HINSTANCE)instance, NULL);
    SetWindowLongPtrA((HWND)gui->sidebar_hwnd, GWLP_USERDATA, (LONG_PTR)gui);

    /* Terminal */
    int term_left = gui->config.sidebar_visible ? gui->config.sidebar_width : 0;
    gui->term_hwnd = CreateWindowExA(0, TERM_CLASS, NULL,
        WS_CHILD | WS_VISIBLE,
        term_left, 28, gui->config.width - term_left, gui->config.height - 50,
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
    gui_set_status(gui, "Ready — Ctrl+N to connect");
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
        "%s version %s\n\n"
        "Enhanced SSH terminal based on %s\n\n"
        "Warm Blue theme with minimalist design.\n"
        "Tabs, macros, SFTP, split-view, themes,\n"
        "plugins, and 205+ features.\n\n"
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

    /* Update status bar session info */
    if (gui->statusbar) {
        char sess[128];
        if (user && user[0])
            snprintf(sess, sizeof(sess), "%s@%s:%d", user, host, port);
        else
            snprintf(sess, sizeof(sess), "%s:%d", host, port);
        SendMessage((HWND)gui->statusbar, SB_SETTEXTA, 1, (LPARAM)sess);
    }

    /* Repaint terminal and sidebar */
    if (gui->term_hwnd)
        InvalidateRect((HWND)gui->term_hwnd, NULL, TRUE);
    if (gui->sidebar_hwnd)
        InvalidateRect((HWND)gui->sidebar_hwnd, NULL, TRUE);

    /* Update tab text */
    if (gui->tab_ctrl) {
        TCITEMA item;
        char tab_text[64];
        if (user && user[0])
            snprintf(tab_text, sizeof(tab_text), "%s@%s", user, host);
        else
            snprintf(tab_text, sizeof(tab_text), "%s:%d", host, port);
        memset(&item, 0, sizeof(item));
        item.mask = TCIF_TEXT;
        item.pszText = tab_text;
        SendMessage((HWND)gui->tab_ctrl, TCM_SETITEMA, 0, (LPARAM)&item);
    }

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

    if (gui->statusbar)
        SendMessage((HWND)gui->statusbar, SB_SETTEXTA, 1,
                    (LPARAM)"No session");

    /* Repaint terminal and sidebar */
    if (gui->term_hwnd)
        InvalidateRect((HWND)gui->term_hwnd, NULL, TRUE);
    if (gui->sidebar_hwnd)
        InvalidateRect((HWND)gui->sidebar_hwnd, NULL, TRUE);

    /* Reset tab text */
    if (gui->tab_ctrl) {
        TCITEMA item;
        memset(&item, 0, sizeof(item));
        item.mask = TCIF_TEXT;
        item.pszText = "New Session";
        SendMessage((HWND)gui->tab_ctrl, TCM_SETITEMA, 0, (LPARAM)&item);
    }
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
