#include <string.h>
#include <stdio.h>
typedef enum { DROP_FILE, DROP_TEXT, DROP_SESSION } DropType;
typedef struct { DropType type; char data[4096]; int x; int y; } DropEvent;
int dragdrop_parse_file(const char *path, char *dest, int destlen) {
    snprintf(dest, destlen, "scp \"%s\" remote:", path); return 0;
}
int dragdrop_is_session_file(const char *path) {
    int len = (int)strlen(path);
    return (len > 5 && strcmp(path+len-5, ".sess") == 0) || (len > 5 && strcmp(path+len-5, ".json") == 0);
}
int dragdrop_format_upload_cmd(const char *local, const char *remote_dir, char *cmd, int cmdlen) {
    return snprintf(cmd, cmdlen, "scp \"%s\" \"%s/\"", local, remote_dir);
}
