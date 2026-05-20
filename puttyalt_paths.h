#ifndef PUTTYALT_PATHS_H
#define PUTTYALT_PATHS_H

#define PATH_MAX_LEN 1024

typedef struct {
    char exe_dir[PATH_MAX_LEN];
    char config_dir[PATH_MAX_LEN];
    char data_dir[PATH_MAX_LEN];
    char log_dir[PATH_MAX_LEN];
    char scripts_dir[PATH_MAX_LEN];
    char snippets_dir[PATH_MAX_LEN];
    char sessions_dir[PATH_MAX_LEN];
    char backups_dir[PATH_MAX_LEN];
    int portable;
} AppPaths;

int  paths_init(AppPaths *ap, int portable);
int  paths_ensure_dirs(const AppPaths *ap);
const char *paths_config_file(const AppPaths *ap, const char *filename, char *buf, int bufsz);
const char *paths_data_file(const AppPaths *ap, const char *filename, char *buf, int bufsz);

#endif
