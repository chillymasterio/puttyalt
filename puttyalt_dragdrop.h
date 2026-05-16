#ifndef PUTTYALT_DRAGDROP_H
#define PUTTYALT_DRAGDROP_H

#define DROP_TYPE_NONE    0
#define DROP_TYPE_FILE    1
#define DROP_TYPE_TEXT    2
#define DROP_TYPE_SESSION 3

typedef struct {
    int enabled;
    int drop_action; /* 0=upload SFTP, 1=paste path, 2=ask */
    int confirm_upload;
    int max_file_size_mb;
    char last_drop_path[512];
    int last_drop_type;
} DragDropHandler;

void dragdrop_init(DragDropHandler *dd);
int  dragdrop_accept(DragDropHandler *dd, const char *path);
int  dragdrop_detect_type(const char *data);
const char *dragdrop_action_name(int action);

#endif
