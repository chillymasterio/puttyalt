#include <string.h>
#include <stdio.h>
int theme_export_json(const char *name, unsigned int bg, unsigned int fg, unsigned int cursor, char *buf, int buflen) {
    return snprintf(buf, buflen, "{\"name\":\"%s\",\"background\":\"#%06x\",\"foreground\":\"#%06x\",\"cursor\":\"#%06x\"}", name, bg, fg, cursor);
}
int theme_import_json(const char *json, char *name, int namelen, unsigned int *bg, unsigned int *fg, unsigned int *cursor) {
    char *p; *bg=0; *fg=0; *cursor=0;
    if ((p=strstr(json,"\"name\":\""))) sscanf(p+8, "%[^\"]", name);
    if ((p=strstr(json,"\"background\":\"#"))) sscanf(p+15, "%6x", bg);
    if ((p=strstr(json,"\"foreground\":\"#"))) sscanf(p+15, "%6x", fg);
    if ((p=strstr(json,"\"cursor\":\"#"))) sscanf(p+11, "%6x", cursor);
    return name[0] ? 0 : -1;
}
