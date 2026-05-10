/*
 * main.c: PuttyAlt GUI application entry point.
 *
 * Win32: WinMain with GUI initialization.
 * Unix: main() with GTK/console fallback.
 */

#include "puttyalt.h"
#include "puttyalt_gui.h"
#include "puttyalt_dialogs.h"
#include "puttyalt_tray.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static GUIState g_gui;
static TrayIcon g_tray;

static void print_version(void)
{
    printf("%s %s (based on %s)\n",
           PUTTYALT_NAME, PUTTYALT_VERSION_STR, PUTTYALT_UPSTREAM);
}

static void print_usage(void)
{
    print_version();
    printf("\nUsage: puttyalt [options] [user@]host[:port]\n\n");
    printf("Options:\n");
    printf("  -P port       Port number (default: 22)\n");
    printf("  -l user       Login username\n");
    printf("  -pw pass      Login password\n");
    printf("  -i keyfile    Private key file (.ppk)\n");
    printf("  -load name    Load saved session\n");
    printf("  -telnet       Use Telnet protocol\n");
    printf("  -serial       Use Serial connection\n");
    printf("  -raw          Use Raw connection\n");
    printf("  -config file  Use alternate config file\n");
    printf("  -portable     Force portable mode\n");
    printf("  -version      Show version and exit\n");
    printf("  -help         Show this help\n");
}

#ifdef _WIN32
#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow)
{
    (void)hPrevInstance;
    (void)nCmdShow;

    ConnectParams params;
    char config_path[512] = "";
    int portable = 0;
    int show_help = 0;
    int show_ver = 0;

    /* Parse command line */
    int argc = __argc;
    char **argv = __argv;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-help") == 0 || strcmp(argv[i], "--help") == 0) {
            show_help = 1;
        } else if (strcmp(argv[i], "-version") == 0 || strcmp(argv[i], "--version") == 0) {
            show_ver = 1;
        } else if (strcmp(argv[i], "-portable") == 0) {
            portable = 1;
        } else if (strcmp(argv[i], "-config") == 0 && i + 1 < argc) {
            snprintf(config_path, sizeof(config_path), "%s", argv[++i]);
        }
    }

    if (show_ver) {
        char msg[128];
        snprintf(msg, sizeof(msg), "%s %s\nBased on %s",
                 PUTTYALT_NAME, PUTTYALT_VERSION_STR, PUTTYALT_UPSTREAM);
        MessageBoxA(NULL, msg, PUTTYALT_NAME, MB_OK | MB_ICONINFORMATION);
        return 0;
    }

    if (show_help) {
        char msg[1024];
        snprintf(msg, sizeof(msg),
            "%s %s\n\n"
            "Usage: puttyalt [options] [user@]host[:port]\n\n"
            "Options:\n"
            "  -P port       Port number\n"
            "  -l user       Login username\n"
            "  -i keyfile    Private key file\n"
            "  -load name    Load saved session\n"
            "  -telnet       Telnet protocol\n"
            "  -serial       Serial connection\n"
            "  -portable     Portable mode\n"
            "  -config file  Alternate config\n",
            PUTTYALT_NAME, PUTTYALT_VERSION_STR);
        MessageBoxA(NULL, msg, PUTTYALT_NAME " - Help", MB_OK | MB_ICONINFORMATION);
        return 0;
    }

    /* Initialize GUI */
    if (gui_init(&g_gui, hInstance) != 0) {
        MessageBoxA(NULL, "Failed to initialize GUI.",
                    PUTTYALT_NAME, MB_OK | MB_ICONERROR);
        return 1;
    }

    /* Load config */
    if (config_path[0] == '\0') {
        if (portable) {
            snprintf(config_path, sizeof(config_path), "puttyalt.conf");
        } else {
            char *appdata = getenv("APPDATA");
            if (appdata)
                snprintf(config_path, sizeof(config_path),
                         "%s\\PuttyAlt\\puttyalt.conf", appdata);
        }
    }

    if (config_path[0])
        gui_config_load(&g_gui.config, config_path);

    /* Init tray icon */
    tray_init(&g_tray, &g_gui);

    /* Auto-connect if host specified on command line */
    if (parse_connect_args(argc, argv, &params) == 0 && params.hostname[0]) {
        gui_connect(&g_gui, params.hostname, params.port, params.username);
    }

    /* Main event loop */
    int ret = gui_run(&g_gui);

    /* Save config on exit */
    if (config_path[0])
        gui_config_save(&g_gui.config, config_path);

    tray_destroy(&g_tray, &g_gui);
    gui_destroy(&g_gui);

    return ret;
}

#else /* Unix */

int main(int argc, char **argv)
{
    ConnectParams params;
    char config_path[512] = "";

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-help") == 0 || strcmp(argv[i], "--help") == 0) {
            print_usage();
            return 0;
        }
        if (strcmp(argv[i], "-version") == 0 || strcmp(argv[i], "--version") == 0) {
            print_version();
            return 0;
        }
        if (strcmp(argv[i], "-config") == 0 && i + 1 < argc) {
            snprintf(config_path, sizeof(config_path), "%s", argv[++i]);
        }
    }

    if (gui_init(&g_gui, NULL) != 0) {
        fprintf(stderr, "%s: failed to initialize GUI\n", PUTTYALT_NAME);
        return 1;
    }

    if (config_path[0] == '\0') {
        char *home = getenv("HOME");
        if (home)
            snprintf(config_path, sizeof(config_path),
                     "%s/.config/puttyalt/puttyalt.conf", home);
    }

    if (config_path[0])
        gui_config_load(&g_gui.config, config_path);

    tray_init(&g_tray, &g_gui);

    if (parse_connect_args(argc, argv, &params) == 0 && params.hostname[0]) {
        gui_connect(&g_gui, params.hostname, params.port, params.username);
    }

    int ret = gui_run(&g_gui);

    if (config_path[0])
        gui_config_save(&g_gui.config, config_path);

    tray_destroy(&g_tray, &g_gui);
    gui_destroy(&g_gui);

    return ret;
}

#endif
