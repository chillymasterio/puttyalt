#include "puttyalt_broadcast_v2.h"
#include <string.h>
#include <stdio.h>

void bcast_init(BroadcastV2 *bc) { memset(bc, 0, sizeof(*bc)); bc->confirm_dangerous = 1; bc->echo_local = 1; }

int bcast_add_target(BroadcastV2 *bc, int session_id)
{
    if (bc->target_count >= BCAST_MAX_TARGETS) return -1;
    for (int i = 0; i < bc->target_count; i++) if (bc->session_ids[i] == session_id) return 0;
    bc->session_ids[bc->target_count++] = session_id;
    return 0;
}

int bcast_remove_target(BroadcastV2 *bc, int session_id)
{
    for (int i = 0; i < bc->target_count; i++) {
        if (bc->session_ids[i] == session_id) {
            memmove(&bc->session_ids[i], &bc->session_ids[i + 1], (bc->target_count - i - 1) * sizeof(int));
            bc->target_count--;
            return 0;
        }
    }
    return -1;
}

void bcast_clear_targets(BroadcastV2 *bc) { bc->target_count = 0; }

static const char *dangerous_cmds[] = {"rm -rf", "mkfs", "dd if=", "shutdown", "reboot", "halt", "> /dev/sd", "wipefs", NULL};

int bcast_is_dangerous(const char *cmd)
{
    for (const char **dc = dangerous_cmds; *dc; dc++)
        if (strstr(cmd, *dc)) return 1;
    return 0;
}

int bcast_send(BroadcastV2 *bc, const char *data, int len)
{
    if (!bc->enabled || bc->target_count == 0) return -1;
    bc->bytes_broadcast += len;
    bc->cmd_count++;
    return bc->target_count;
}

void bcast_toggle(BroadcastV2 *bc) { bc->enabled = !bc->enabled; }
