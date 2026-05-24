#ifndef PUTTYALT_QUICKCMD_H
#define PUTTYALT_QUICKCMD_H

#define QCMD_MAX 24

typedef struct {
    char label[32];
    char command[512];
    char icon[16]; /* emoji or short icon code */
    char tooltip[128];
    int  confirm;
    int  color;
} QuickCommand;

typedef struct {
    QuickCommand items[QCMD_MAX];
    int count;
    int visible;
    int orientation; /* 0=horizontal, 1=vertical */
    int icon_only;
} QuickCmdPanel;

void quickcmd_init(QuickCmdPanel *qp);
void quickcmd_add_defaults(QuickCmdPanel *qp);
int  quickcmd_add(QuickCmdPanel *qp, const char *label, const char *cmd, int confirm);
int  quickcmd_remove(QuickCmdPanel *qp, int index);
void quickcmd_toggle(QuickCmdPanel *qp);
QuickCommand *quickcmd_get(QuickCmdPanel *qp, int index);

#endif
