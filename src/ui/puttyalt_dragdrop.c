#include "puttyalt_dragdrop.h"
#include <string.h>
#include <stdio.h>

void dragdrop_init(DragDropHandler *dd)
{
    memset(dd, 0, sizeof(*dd));
    dd->enabled = 1;
    dd->drop_action = 2; /* ask by default */
    dd->confirm_upload = 1;
    dd->max_file_size_mb = 100;
}

int dragdrop_accept(DragDropHandler *dd, const char *path)
{
    if (!dd->enabled || !path || !path[0]) return 0;
    snprintf(dd->last_drop_path, sizeof(dd->last_drop_path), "%s", path);
    dd->last_drop_type = DROP_TYPE_FILE;
    return 1;
}

int dragdrop_detect_type(const char *data)
{
    if (!data) return DROP_TYPE_NONE;
    /* simple heuristic: if it looks like a path */
    if (data[0] == '/' || data[0] == '\\' ||
        (data[0] && data[1] == ':')) return DROP_TYPE_FILE;
    return DROP_TYPE_TEXT;
}

const char *dragdrop_action_name(int action)
{
    switch (action) {
        case 0: return "Upload via SFTP";
        case 1: return "Paste file path";
        case 2: return "Ask each time";
        default: return "Unknown";
    }
}
