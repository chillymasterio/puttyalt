/*
 * puttyalt_cmdhistory.c: Command history implementation.
 */

#include "puttyalt_cmdhistory.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

void cmdhistory_init(CmdHistory *ch)
{
    memset(ch, 0, sizeof(*ch));
    ch->cursor = -1;
}

void cmdhistory_add(CmdHistory *ch, const char *cmd, const char *session)
{
    if (!cmd || !*cmd) return;

    /* Deduplicate consecutive entries */
    if (ch->count > 0 &&
        strcmp(ch->entries[ch->count - 1].command, cmd) == 0)
        return;

    if (ch->count >= CMD_HISTORY_SIZE) {
        /* Shift everything down, dropping oldest */
        memmove(&ch->entries[0], &ch->entries[1],
                sizeof(CmdHistoryEntry) * (CMD_HISTORY_SIZE - 1));
        ch->count = CMD_HISTORY_SIZE - 1;
    }

    CmdHistoryEntry *e = &ch->entries[ch->count];
    snprintf(e->command, CMD_MAX_LENGTH, "%s", cmd);
    if (session)
        snprintf(e->session, sizeof(e->session), "%s", session);
    e->timestamp = (long)time(NULL);
    e->exit_code = -1;
    ch->count++;
    ch->cursor = ch->count;
}

const char *cmdhistory_prev(CmdHistory *ch)
{
    if (ch->count == 0) return NULL;
    if (ch->cursor > 0) ch->cursor--;
    return ch->entries[ch->cursor].command;
}

const char *cmdhistory_next(CmdHistory *ch)
{
    if (ch->cursor >= ch->count - 1) {
        ch->cursor = ch->count;
        return NULL;  /* at end = empty line */
    }
    ch->cursor++;
    return ch->entries[ch->cursor].command;
}

int cmdhistory_search(const CmdHistory *ch, const char *prefix,
                      const CmdHistoryEntry **results, int max)
{
    int n = 0;
    /* Search backwards for most recent matches first */
    for (int i = ch->count - 1; i >= 0 && n < max; i--) {
        if (strncmp(ch->entries[i].command, prefix, strlen(prefix)) == 0)
            results[n++] = &ch->entries[i];
    }
    return n;
}

int cmdhistory_load(CmdHistory *ch, const char *path)
{
    FILE *f = fopen(path, "r");
    char line[CMD_MAX_LENGTH + 256];

    if (!f) return -1;
    cmdhistory_init(ch);

    while (fgets(line, sizeof(line), f) && ch->count < CMD_HISTORY_SIZE) {
        size_t len = strlen(line);
        while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r'))
            line[--len] = '\0';
        if (!*line) continue;

        CmdHistoryEntry *e = &ch->entries[ch->count++];
        memset(e, 0, sizeof(*e));
        snprintf(e->command, CMD_MAX_LENGTH, "%s", line);
        e->exit_code = -1;
    }

    fclose(f);
    ch->cursor = ch->count;
    return 0;
}

int cmdhistory_save(const CmdHistory *ch, const char *path)
{
    FILE *f = fopen(path, "w");
    if (!f) return -1;
    for (int i = 0; i < ch->count; i++)
        fprintf(f, "%s\n", ch->entries[i].command);
    fclose(f);
    return 0;
}

void cmdhistory_clear(CmdHistory *ch)
{
    ch->count = 0;
    ch->cursor = -1;
}
