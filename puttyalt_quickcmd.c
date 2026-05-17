#include "puttyalt_quickcmd.h"
#include <string.h>

void quickcmd_init(QuickCmdPanel *qp)
{
    memset(qp, 0, sizeof(*qp));
    qp->visible = 1;
    qp->orientation = 0;
}

int quickcmd_add(QuickCmdPanel *qp, const char *label, const char *cmd, int confirm)
{
    if (qp->count >= QCMD_MAX) return -1;
    QuickCommand *q = &qp->items[qp->count++];
    memset(q, 0, sizeof(*q));
    snprintf(q->label, sizeof(q->label), "%s", label);
    snprintf(q->command, sizeof(q->command), "%s", cmd);
    q->confirm = confirm;
    return qp->count - 1;
}

void quickcmd_add_defaults(QuickCmdPanel *qp)
{
    quickcmd_add(qp, "Top", "top -bn1 | head -20", 0);
    quickcmd_add(qp, "Disk", "df -h", 0);
    quickcmd_add(qp, "Memory", "free -m", 0);
    quickcmd_add(qp, "Network", "ss -tunap | head -20", 0);
    quickcmd_add(qp, "Uptime", "uptime", 0);
    quickcmd_add(qp, "Who", "who", 0);
    quickcmd_add(qp, "Last", "last -10", 0);
    quickcmd_add(qp, "Restart", "sudo systemctl restart", 1);
}

int quickcmd_remove(QuickCmdPanel *qp, int index)
{
    if (index < 0 || index >= qp->count) return -1;
    memmove(&qp->items[index], &qp->items[index+1], (qp->count - index - 1) * sizeof(QuickCommand));
    qp->count--;
    return 0;
}

void quickcmd_toggle(QuickCmdPanel *qp) { qp->visible = !qp->visible; }
QuickCommand *quickcmd_get(QuickCmdPanel *qp, int index) { return (index >= 0 && index < qp->count) ? &qp->items[index] : NULL; }
