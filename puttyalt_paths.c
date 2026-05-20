#include "puttyalt_paths.h"
#include <string.h>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#include <shlobj.h>
#else
#include <unistd.h>
#include <sys/stat.h>
#include <pwd.h>
#endif

static void ensure_trailing_sep(char *path)
{
    size_t len = strlen(path);
    if (len > 0 && path[len-1] != '/' && path[len-1] != '\\') {
#ifdef _WIN32
        if (len < PATH_MAX_LEN - 1) { path[len] = '\\'; path[len+1] = '\0'; }
#else
        if (len < PATH_MAX_LEN - 1) { path[len] = '/'; path[len+1] = '\0'; }
#endif
    }
}

int paths_init(AppPaths *ap, int portable)
{
    memset(ap, 0, sizeof(*ap));
    ap->portable = portable;

#ifdef _WIN32
    /* Get exe directory */
    GetModuleFileNameA(NULL, ap->exe_dir, PATH_MAX_LEN);
    char *last_sep = strrchr(ap->exe_dir, '\\');
    if (last_sep) *(last_sep + 1) = '\0';

    if (portable) {
        snprintf(ap->config_dir, PATH_MAX_LEN, "%sconfig\\", ap->exe_dir);
        snprintf(ap->data_dir, PATH_MAX_LEN, "%sdata\\", ap->exe_dir);
        snprintf(ap->log_dir, PATH_MAX_LEN, "%slogs\\", ap->exe_dir);
    } else {
        char appdata[PATH_MAX_LEN];
        if (SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, appdata) == S_OK) {
            snprintf(ap->config_dir, PATH_MAX_LEN, "%s\\PuttyAlt\\", appdata);
            snprintf(ap->data_dir, PATH_MAX_LEN, "%s\\PuttyAlt\\data\\", appdata);
            snprintf(ap->log_dir, PATH_MAX_LEN, "%s\\PuttyAlt\\logs\\", appdata);
        }
    }
#else
    /* Unix: use XDG or ~/.config */
    const char *home = getenv("HOME");
    if (!home) {
        struct passwd *pw = getpwuid(getuid());
        home = pw ? pw->pw_dir : "/tmp";
    }
    snprintf(ap->exe_dir, PATH_MAX_LEN, "%s/.local/bin/", home);

    const char *xdg_config = getenv("XDG_CONFIG_HOME");
    if (xdg_config && xdg_config[0])
        snprintf(ap->config_dir, PATH_MAX_LEN, "%s/puttyalt/", xdg_config);
    else
        snprintf(ap->config_dir, PATH_MAX_LEN, "%s/.config/puttyalt/", home);

    const char *xdg_data = getenv("XDG_DATA_HOME");
    if (xdg_data && xdg_data[0])
        snprintf(ap->data_dir, PATH_MAX_LEN, "%s/puttyalt/", xdg_data);
    else
        snprintf(ap->data_dir, PATH_MAX_LEN, "%s/.local/share/puttyalt/", home);

    snprintf(ap->log_dir, PATH_MAX_LEN, "%slogs/", ap->data_dir);
#endif

    snprintf(ap->scripts_dir, PATH_MAX_LEN, "%sscripts%c",
             ap->data_dir, ap->data_dir[0] == '/' ? '/' : '\\');
    snprintf(ap->snippets_dir, PATH_MAX_LEN, "%ssnippets%c",
             ap->data_dir, ap->data_dir[0] == '/' ? '/' : '\\');
    snprintf(ap->sessions_dir, PATH_MAX_LEN, "%ssessions%c",
             ap->config_dir, ap->config_dir[0] == '/' ? '/' : '\\');
    snprintf(ap->backups_dir, PATH_MAX_LEN, "%sbackups%c",
             ap->data_dir, ap->data_dir[0] == '/' ? '/' : '\\');

    return 0;
}

int paths_ensure_dirs(const AppPaths *ap)
{
    const char *dirs[] = {
        ap->config_dir, ap->data_dir, ap->log_dir,
        ap->scripts_dir, ap->snippets_dir, ap->sessions_dir,
        ap->backups_dir
    };
#ifdef _WIN32
    for (int i = 0; i < 7; i++)
        CreateDirectoryA(dirs[i], NULL);
#else
    for (int i = 0; i < 7; i++)
        mkdir(dirs[i], 0700);
#endif
    return 0;
}

const char *paths_config_file(const AppPaths *ap, const char *filename,
                              char *buf, int bufsz)
{
    snprintf(buf, bufsz, "%s%s", ap->config_dir, filename);
    return buf;
}

const char *paths_data_file(const AppPaths *ap, const char *filename,
                            char *buf, int bufsz)
{
    snprintf(buf, bufsz, "%s%s", ap->data_dir, filename);
    return buf;
}
