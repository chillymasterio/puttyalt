#ifndef PUTTYALT_PORTABLE_V2_H
#define PUTTYALT_PORTABLE_V2_H

typedef struct {
    int is_portable;
    char exe_dir[512];
    char config_dir[512];
    char data_dir[512];
    char log_dir[512];
    char plugin_dir[512];
    char theme_dir[512];
} PortablePaths;

void portable_detect(PortablePaths *pp);
void portable_ensure_dirs(PortablePaths *pp);
const char *portable_config_path(PortablePaths *pp, const char *filename, char *buf, int buflen);
const char *portable_data_path(PortablePaths *pp, const char *filename, char *buf, int buflen);

#endif
