/* puttyalt_oscstatus.c - OSC status line parsing (title/icon/working dir). */
#include <string.h>
#include <stdio.h>
typedef struct {
    char title[128];
    char icon_name[64];
    char working_dir[160];
} OscStatus;
void oscstatus_init(OscStatus *o) {
    if (o) memset(o, 0, sizeof(*o));
}
int oscstatus_handle(OscStatus *o, int command, const char *param) {
    if (!o || !param) return -1;
    switch (command) {
        case 0:
            snprintf(o->title, 128, "%s", param);
            snprintf(o->icon_name, 64, "%s", param);
            return 0;
        case 1:
            snprintf(o->icon_name, 64, "%s", param);
            return 0;
        case 2:
            snprintf(o->title, 128, "%s", param);
            return 0;
        case 7:
            /* working directory: file://host/path */
            {
                const char *path = strstr(param, "://");
                if (path) {
                    path += 3;
                    const char *slash = strchr(path, '/');
                    snprintf(o->working_dir, 160, "%s", slash ? slash : path);
                } else {
                    snprintf(o->working_dir, 160, "%s", param);
                }
            }
            return 0;
        default:
            return -1;
    }
}
const char *oscstatus_title(const OscStatus *o) {
    return o ? o->title : "";
}
const char *oscstatus_workdir(const OscStatus *o) {
    return o ? o->working_dir : "";
}
