/* puttyalt_cmdfreqmodel.c - command frequency model for the suggestion ranker.
 * Maintains per-command usage counts with time decay and persistence. */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define CMDFREQ_MAX_CMD   64    /* max stored distinct commands */
#define CMDFREQ_CMD_LEN   48    /* max chars (incl. NUL) per command string */
#define CMDFREQ_HALFLIFE  86400UL /* decay half-life in ms units (caller-defined) */

typedef struct CmdFreqEntry {
    char cmd[CMDFREQ_CMD_LEN];
    float count;            /* decayed usage weight */
    unsigned long last_ms;  /* timestamp of last update */
} CmdFreqEntry;

typedef struct CmdFreqModel {
    CmdFreqEntry entries[CMDFREQ_MAX_CMD];
    int n;                  /* number of live entries */
    unsigned long last_decay_ms;
} CmdFreqModel;

void cmdfreq_init(CmdFreqModel *m) {
    if (!m) return;
    memset(m, 0, sizeof(*m));
    m->n = 0;
    m->last_decay_ms = 0UL;
}

static int cmdfreq_find(const CmdFreqModel *m, const char *cmd) {
    int i;
    for (i = 0; i < m->n; i++)
        if (strncmp(m->entries[i].cmd, cmd, CMDFREQ_CMD_LEN) == 0)
            return i;
    return -1;
}

void cmdfreq_record(CmdFreqModel *m, const char *cmd) {
    int i, victim;
    if (!m || !cmd || cmd[0] == '\0') return;
    i = cmdfreq_find(m, cmd);
    if (i >= 0) {
        m->entries[i].count += 1.0f;
        return;
    }
    if (m->n < CMDFREQ_MAX_CMD) {
        i = m->n++;
    } else {
        /* evict the lowest-count entry to make room */
        int k;
        victim = 0;
        for (k = 1; k < m->n; k++)
            if (m->entries[k].count < m->entries[victim].count)
                victim = k;
        i = victim;
    }
    memset(&m->entries[i], 0, sizeof(m->entries[i]));
    strncpy(m->entries[i].cmd, cmd, CMDFREQ_CMD_LEN - 1);
    m->entries[i].cmd[CMDFREQ_CMD_LEN - 1] = '\0';
    m->entries[i].count = 1.0f;
}

float cmdfreq_score(const CmdFreqModel *m, const char *cmd) {
    int i;
    if (!m || !cmd) return 0.0f;
    i = cmdfreq_find(m, cmd);
    return (i >= 0) ? m->entries[i].count : 0.0f;
}

void cmdfreq_decay(CmdFreqModel *m, unsigned long now) {
    int i, w;
    unsigned long elapsed;
    float factor;
    if (!m) return;
    if (now < m->last_decay_ms) { m->last_decay_ms = now; return; }
    elapsed = now - m->last_decay_ms;
    /* approximate exponential half-life decay without math.h */
    factor = 1.0f;
    while (elapsed >= CMDFREQ_HALFLIFE && factor > 0.0009765625f) {
        factor *= 0.5f;
        elapsed -= CMDFREQ_HALFLIFE;
    }
    if (CMDFREQ_HALFLIFE > 0UL)
        factor *= (1.0f - 0.5f * ((float)elapsed / (float)CMDFREQ_HALFLIFE));
    if (factor < 0.0f) factor = 0.0f;
    w = 0;
    for (i = 0; i < m->n; i++) {
        m->entries[i].count *= factor;
        if (m->entries[i].count >= 0.01f) {
            if (w != i) m->entries[w] = m->entries[i];
            w++;
        }
    }
    m->n = w;
    m->last_decay_ms = now;
}

static void cmdfreq_swap(CmdFreqEntry *a, CmdFreqEntry *b) {
    CmdFreqEntry t = *a; *a = *b; *b = t;
}

int cmdfreq_top(const CmdFreqModel *m, CmdFreqEntry *out, int outcap) {
    CmdFreqModel tmp;
    int i, j, count;
    if (!m || !out || outcap <= 0) return -1;
    tmp = *m;  /* local copy; sort by descending count */
    for (i = 0; i < tmp.n; i++)
        for (j = i + 1; j < tmp.n; j++)
            if (tmp.entries[j].count > tmp.entries[i].count)
                cmdfreq_swap(&tmp.entries[i], &tmp.entries[j]);
    count = (tmp.n < outcap) ? tmp.n : outcap;
    for (i = 0; i < count; i++)
        out[i] = tmp.entries[i];
    return count;
}

int cmdfreq_serialize(const CmdFreqModel *m, char *out, size_t outcap) {
    int i;
    size_t off = 0;
    int w;
    if (!m || !out || outcap == 0) return -1;
    w = snprintf(out, outcap, "cmdfreq v1 n=%d t=%lu\n", m->n, m->last_decay_ms);
    if (w < 0 || (size_t)w >= outcap) return -1;
    off = (size_t)w;
    for (i = 0; i < m->n; i++) {
        w = snprintf(out + off, outcap - off, "%s\t%.3f\t%lu\n",
                     m->entries[i].cmd, m->entries[i].count,
                     m->entries[i].last_ms);
        if (w < 0 || (size_t)w >= outcap - off) return -1;
        off += (size_t)w;
    }
    return 0;
}
