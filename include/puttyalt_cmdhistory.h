/*
 * puttyalt_cmdhistory.h: Cross-session command history.
 *
 * Persists terminal command history across sessions, with search
 * and auto-complete integration.
 */

#ifndef PUTTYALT_CMDHISTORY_H
#define PUTTYALT_CMDHISTORY_H

#define CMD_HISTORY_SIZE 2048
#define CMD_MAX_LENGTH   4096

typedef struct CmdHistoryEntry {
    char command[CMD_MAX_LENGTH];
    char session[128];
    long timestamp;
    int  exit_code;     /* -1 if unknown */
} CmdHistoryEntry;

typedef struct CmdHistory {
    CmdHistoryEntry entries[CMD_HISTORY_SIZE];
    int count;
    int cursor;         /* for up/down navigation */
} CmdHistory;

void cmdhistory_init(CmdHistory *ch);
void cmdhistory_add(CmdHistory *ch, const char *cmd, const char *session);
const char *cmdhistory_prev(CmdHistory *ch);
const char *cmdhistory_next(CmdHistory *ch);
int cmdhistory_search(const CmdHistory *ch, const char *prefix,
                      const CmdHistoryEntry **results, int max);
int cmdhistory_load(CmdHistory *ch, const char *path);
int cmdhistory_save(const CmdHistory *ch, const char *path);
void cmdhistory_clear(CmdHistory *ch);

#endif /* PUTTYALT_CMDHISTORY_H */
