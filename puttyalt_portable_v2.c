#include "puttyalt_portable_v2.h"
#include <string.h>
#include <stdio.h>

void portable_detect(PortablePaths *pp)
{
    memset(pp, 0, sizeof(*pp));
    FILE *f = fopen("puttyalt.ini", "r");
    if (f) { fclose(f); pp->is_portable = 1; }

    if (pp->is_portable) {
        snprintf(pp->exe_dir, sizeof(pp->exe_dir), ".");
        snprintf(pp->config_dir, sizeof(pp->config_dir), "./config");
        snprintf(pp->data_dir, sizeof(pp->data_dir), "./data");
        snprintf(pp->log_dir, sizeof(pp->log_dir), "./logs");
        snprintf(pp->plugin_dir, sizeof(pp->plugin_dir), "./plugins");
        snprintf(pp->theme_dir, sizeof(pp->theme_dir), "./themes");
    } else {
        const char *home = getenv("HOME");
        const char *appdata = getenv("APPDATA");
        const char *base = appdata ? appdata : (home ? home : ".");
        snprintf(pp->config_dir, sizeof(pp->config_dir), "%s/PuttyAlt", base);
        snprintf(pp->data_dir, sizeof(pp->data_dir), "%s/PuttyAlt/data", base);
        snprintf(pp->log_dir, sizeof(pp->log_dir), "%s/PuttyAlt/logs", base);
        snprintf(pp->plugin_dir, sizeof(pp->plugin_dir), "%s/PuttyAlt/plugins", base);
        snprintf(pp->theme_dir, sizeof(pp->theme_dir), "%s/PuttyAlt/themes", base);
    }
}

void portable_ensure_dirs(PortablePaths *pp) { (void)pp; }

const char *portable_config_path(PortablePaths *pp, const char *filename, char *buf, int buflen)
{
    snprintf(buf, buflen, "%s/%s", pp->config_dir, filename);
    return buf;
}

const char *portable_data_path(PortablePaths *pp, const char *filename, char *buf, int buflen)
{
    snprintf(buf, buflen, "%s/%s", pp->data_dir, filename);
    return buf;
}
