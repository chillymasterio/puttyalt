#ifndef PUTTYALT_QUICKLAUNCH_H
#define PUTTYALT_QUICKLAUNCH_H

#define QL_MAX_ITEMS 32
#define QL_NAME_LEN  64
#define QL_CMD_LEN   512

typedef enum {
    QL_SESSION,
    QL_SNIPPET,
    QL_SCRIPT,
    QL_BOOKMARK,
    QL_COMMAND
} QuickLaunchType;

typedef struct {
    char name[QL_NAME_LEN];
    char target[QL_CMD_LEN];
    QuickLaunchType type;
    char icon[8];       /* emoji or char */
    char shortcut[32];  /* e.g. "Ctrl+1" */
    int position;
    int uses;
} QuickLaunchItem;

typedef struct {
    QuickLaunchItem items[QL_MAX_ITEMS];
    int count;
    int visible;
} QuickLaunchBar;

void qlbar_init(QuickLaunchBar *ql);
int  qlbar_add(QuickLaunchBar *ql, const QuickLaunchItem *item);
int  qlbar_remove(QuickLaunchBar *ql, const char *name);
int  qlbar_activate(QuickLaunchBar *ql, int index);
int  qlbar_move(QuickLaunchBar *ql, int from, int to);
int  qlbar_save(const QuickLaunchBar *ql, const char *path);
int  qlbar_load(QuickLaunchBar *ql, const char *path);

#endif
