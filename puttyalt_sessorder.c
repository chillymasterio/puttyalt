#include "puttyalt_sessorder.h"
#include <string.h>
#include <stdio.h>

int sessorder_init(SessionOrder *so)
{
    memset(so, 0, sizeof(*so));
    return 0;
}

int sessorder_add(SessionOrder *so, const char *id, int position)
{
    if (so->count >= SESS_ORDER_MAX || !id) return -1;
    snprintf(so->session_ids[so->count], 64, "%s", id);
    so->positions[so->count] = position >= 0 ? position : so->count;
    so->count++;
    so->dirty = 1;
    return so->count - 1;
}

int sessorder_move(SessionOrder *so, const char *id, int new_pos)
{
    for (int i = 0; i < so->count; i++) {
        if (strcmp(so->session_ids[i], id) == 0) {
            int old_pos = so->positions[i];
            so->positions[i] = new_pos;
            /* Shift others */
            for (int j = 0; j < so->count; j++) {
                if (j == i) continue;
                if (so->positions[j] >= new_pos && so->positions[j] < old_pos)
                    so->positions[j]++;
                else if (so->positions[j] <= new_pos && so->positions[j] > old_pos)
                    so->positions[j]--;
            }
            so->dirty = 1;
            return 0;
        }
    }
    return -1;
}

int sessorder_remove(SessionOrder *so, const char *id)
{
    for (int i = 0; i < so->count; i++) {
        if (strcmp(so->session_ids[i], id) == 0) {
            for (int j = i; j < so->count - 1; j++) {
                memcpy(so->session_ids[j], so->session_ids[j+1], 64);
                so->positions[j] = so->positions[j+1];
            }
            so->count--;
            so->dirty = 1;
            return 0;
        }
    }
    return -1;
}

int sessorder_get_pos(const SessionOrder *so, const char *id)
{
    for (int i = 0; i < so->count; i++) {
        if (strcmp(so->session_ids[i], id) == 0)
            return so->positions[i];
    }
    return -1;
}

int sessorder_save(const SessionOrder *so, const char *path)
{
    FILE *f = fopen(path, "w");
    if (!f) return -1;
    for (int i = 0; i < so->count; i++)
        fprintf(f, "%d\t%s\n", so->positions[i], so->session_ids[i]);
    fclose(f);
    return 0;
}

int sessorder_load(SessionOrder *so, const char *path)
{
    FILE *f = fopen(path, "r");
    if (!f) return -1;
    char line[256];
    so->count = 0;
    while (fgets(line, sizeof(line), f) && so->count < SESS_ORDER_MAX) {
        int pos;
        char id[64];
        if (sscanf(line, "%d\t%63s", &pos, id) == 2) {
            snprintf(so->session_ids[so->count], 64, "%s", id);
            so->positions[so->count] = pos;
            so->count++;
        }
    }
    fclose(f);
    so->dirty = 0;
    return 0;
}

void sessorder_destroy(SessionOrder *so)
{
    memset(so, 0, sizeof(*so));
}
