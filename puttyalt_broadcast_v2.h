#ifndef PUTTYALT_BROADCAST_V2_H
#define PUTTYALT_BROADCAST_V2_H

#define BCAST_MAX_TARGETS 32

typedef struct {
    int session_ids[BCAST_MAX_TARGETS];
    int target_count;
    int enabled;
    int exclude_locked;
    int confirm_dangerous;
    char filter_pattern[256];
    int echo_local;
    unsigned long bytes_broadcast;
    int cmd_count;
} BroadcastV2;

void bcast_init(BroadcastV2 *bc);
int  bcast_add_target(BroadcastV2 *bc, int session_id);
int  bcast_remove_target(BroadcastV2 *bc, int session_id);
void bcast_clear_targets(BroadcastV2 *bc);
int  bcast_send(BroadcastV2 *bc, const char *data, int len);
int  bcast_is_dangerous(const char *cmd);
void bcast_toggle(BroadcastV2 *bc);

#endif
