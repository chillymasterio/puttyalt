/*
 * puttyalt_dialogs.c: Settings and connection dialog implementations.
 */

#include "puttyalt_dialogs.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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

static INT_PTR CALLBACK connect_dlgproc(HWND hwnd, UINT msg,
                                         WPARAM wparam, LPARAM lparam)
{
    ConnectParams *params;

    switch (msg) {
    case WM_INITDIALOG:
        SetWindowLongPtrA(hwnd, GWLP_USERDATA, lparam);
        params = (ConnectParams *)lparam;

        /* Pre-fill fields if we have previous values */
        if (params->hostname[0])
            SetDlgItemTextA(hwnd, IDC_HOST, params->hostname);
        SetDlgItemInt(hwnd, IDC_PORT, params->port, FALSE);
        if (params->username[0])
            SetDlgItemTextA(hwnd, IDC_USER, params->username);

        /* Protocol combo */
        SendDlgItemMessage(hwnd, IDC_PROTOCOL, CB_ADDSTRING, 0, (LPARAM)"SSH");
        SendDlgItemMessage(hwnd, IDC_PROTOCOL, CB_ADDSTRING, 0, (LPARAM)"Telnet");
        SendDlgItemMessage(hwnd, IDC_PROTOCOL, CB_ADDSTRING, 0, (LPARAM)"Serial");
        SendDlgItemMessage(hwnd, IDC_PROTOCOL, CB_ADDSTRING, 0, (LPARAM)"Raw");
        SendDlgItemMessage(hwnd, IDC_PROTOCOL, CB_SETCURSEL, params->protocol, 0);

        SetFocus(GetDlgItem(hwnd, IDC_HOST));
        return FALSE;

    case WM_COMMAND:
        params = (ConnectParams *)GetWindowLongPtrA(hwnd, GWLP_USERDATA);

        switch (LOWORD(wparam)) {
        case IDC_CONNECT:
        case IDOK:
            GetDlgItemTextA(hwnd, IDC_HOST, params->hostname,
                            sizeof(params->hostname));
            params->port = GetDlgItemInt(hwnd, IDC_PORT, NULL, FALSE);
            GetDlgItemTextA(hwnd, IDC_USER, params->username,
                            sizeof(params->username));
            GetDlgItemTextA(hwnd, IDC_PASS, params->password,
                            sizeof(params->password));
            GetDlgItemTextA(hwnd, IDC_KEYFILE, params->keyfile,
                            sizeof(params->keyfile));
            params->protocol = (int)SendDlgItemMessage(hwnd, IDC_PROTOCOL,
                CB_GETCURSEL, 0, 0);
            params->save_session = IsDlgButtonChecked(hwnd, IDC_SAVE_SESS);
            if (params->save_session)
                GetDlgItemTextA(hwnd, IDC_SESS_NAME, params->session_name,
                                sizeof(params->session_name));

            if (params->hostname[0] == '\0') {
                MessageBoxA(hwnd, "Please enter a hostname.",
                            PUTTYALT_NAME, MB_OK | MB_ICONWARNING);
                return TRUE;
            }

            EndDialog(hwnd, IDOK);
            return TRUE;

        case IDCANCEL:
            EndDialog(hwnd, IDCANCEL);
            return TRUE;

        case IDC_BROWSE:
            {
                OPENFILENAMEA ofn;
                char path[512] = "";
                memset(&ofn, 0, sizeof(ofn));
                ofn.lStructSize = sizeof(ofn);
                ofn.hwndOwner = hwnd;
                ofn.lpstrFilter = "PuTTY Keys (*.ppk)\0*.ppk\0"
                                  "All Files (*.*)\0*.*\0";
                ofn.lpstrFile = path;
                ofn.nMaxFile = sizeof(path);
                ofn.Flags = OFN_FILEMUSTEXIST;
                if (GetOpenFileNameA(&ofn))
                    SetDlgItemTextA(hwnd, IDC_KEYFILE, path);
            }
            return TRUE;

        case IDC_PROTOCOL:
            if (HIWORD(wparam) == CBN_SELCHANGE) {
                int proto = (int)SendDlgItemMessage(hwnd, IDC_PROTOCOL,
                    CB_GETCURSEL, 0, 0);
                int default_port = (proto == 0) ? 22 :
                                   (proto == 1) ? 23 : 0;
                if (default_port)
                    SetDlgItemInt(hwnd, IDC_PORT, default_port, FALSE);
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
    /* In a real build, this would use a dialog resource.
       For now, create a simple programmatic dialog. */
    (void)gui; (void)params;
    return -1; /* TODO: implement with CreateDialogIndirect */
}

int dialog_settings(GUIState *gui)
{
    (void)gui;
    return -1;
}

int dialog_snippet_manager(GUIState *gui)
{
    (void)gui;
    return -1;
}

int dialog_keygen(GUIState *gui)
{
    (void)gui;
    return -1;
}

int dialog_tunnel_manager(GUIState *gui)
{
    (void)gui;
    return -1;
}

int dialog_bookmark_manager(GUIState *gui)
{
    (void)gui;
    return -1;
}

int dialog_theme_select(GUIState *gui)
{
    (void)gui;
    return -1;
}

int dialog_find(GUIState *gui, char *search_text, int max_len)
{
    (void)gui; (void)search_text; (void)max_len;
    return -1;
}

#else /* Unix stubs */

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
