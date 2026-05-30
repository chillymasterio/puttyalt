/* puttyalt_cmdsuggest.c - local command suggestion ranker for PuttyAlt.
 * Blends recency, frequency, prefix affinity and context into top-N order. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#define CMDSUG_MAXCAND 256
#define CMDSUG_HISTORY 64
#define CMDSUG_CMDLEN  128

typedef struct {
    char     cmd[CMDSUG_CMDLEN];  /* candidate command text */
    uint32_t freq;                /* times previously chosen */
    uint64_t last_ms;             /* last use timestamp (ms) */
    uint64_t now_ms;              /* reference "current" time for recency */
    int      context_match;       /* nonzero if context-relevant */
} CmdCandidate;

typedef struct { char cmd[CMDSUG_CMDLEN]; float score; } CmdRanked;

typedef struct {
    float    w_recency, w_freq, w_affinity;
    char     history[CMDSUG_HISTORY][CMDSUG_CMDLEN];
    uint32_t hist_freq[CMDSUG_HISTORY];
    int      hist_count;
} CmdSuggestRanker;

void cmdsug_init(CmdSuggestRanker *r) {
    if (!r) return;
    memset(r, 0, sizeof(*r));
    r->w_recency = 0.4f; r->w_freq = 0.4f; r->w_affinity = 0.2f;
}

void cmdsug_set_weights(CmdSuggestRanker *r, float recency, float freq, float affinity) {
    if (!r) return;
    if (recency < 0.0f) recency = 0.0f;
    if (freq < 0.0f) freq = 0.0f;
    if (affinity < 0.0f) affinity = 0.0f;
    float sum = recency + freq + affinity;
    if (sum <= 0.0f) { recency = freq = affinity = 1.0f; sum = 3.0f; }
    r->w_recency = recency / sum;
    r->w_freq = freq / sum;
    r->w_affinity = affinity / sum;
}

static float cmdsug_affinity(const char *cmd, const char *prefix) {
    size_t pl, i;
    if (!prefix || !prefix[0]) return 0.5f;            /* no prefix: neutral */
    pl = strlen(prefix);
    for (i = 0; i < pl; i++)
        if (cmd[i] == '\0' || cmd[i] != prefix[i])
            return (float)i / (float)(pl + 1);         /* partial run scaled */
    return 1.0f;                                       /* full prefix match */
}

float cmdsug_score(CmdSuggestRanker *r, const CmdCandidate *c, const char *prefix) {
    if (!r || !c) return 0.0f;
    float rec = 0.0f;                                  /* recency: 1h window */
    if (c->now_ms >= c->last_ms) {
        uint64_t age = c->now_ms - c->last_ms;
        if (age < 3600000ULL) rec = 1.0f - (float)age / 3600000.0f;
    }
    float fr = (float)c->freq / (float)(c->freq + 4u); /* saturating freq */
    float aff = cmdsug_affinity(c->cmd, prefix);
    float score = r->w_recency * rec + r->w_freq * fr + r->w_affinity * aff;
    if (c->context_match) score += 0.15f;              /* context bonus */
    return score > 1.0f ? 1.0f : score;
}

int cmdsug_rank(CmdSuggestRanker *r, const char *prefix, const CmdCandidate *cands,
                int n, CmdRanked *out, int outcap) {
    int i, j, written = 0;
    if (!r || !cands || !out || n < 0 || outcap <= 0) return -1;
    if (n > CMDSUG_MAXCAND) n = CMDSUG_MAXCAND;
    for (i = 0; i < n; i++) {
        float s = cmdsug_score(r, &cands[i], prefix);
        int pos = written;
        for (j = 0; j < written; j++)
            if (s > out[j].score) { pos = j; break; }
        if (pos >= outcap) continue;                   /* below the cut */
        int last = (written < outcap) ? written : outcap - 1;
        for (j = last; j > pos; j--) out[j] = out[j - 1];
        snprintf(out[pos].cmd, CMDSUG_CMDLEN, "%s", cands[i].cmd);
        out[pos].score = s;
        if (written < outcap) written++;
    }
    return written;
}

void cmdsug_feedback(CmdSuggestRanker *r, const char *chosen) {
    int i, min_i = 0;
    if (!r || !chosen || !chosen[0]) return;
    for (i = 0; i < r->hist_count; i++)
        if (strncmp(r->history[i], chosen, CMDSUG_CMDLEN) == 0) { r->hist_freq[i]++; return; }
    if (r->hist_count < CMDSUG_HISTORY) {
        snprintf(r->history[r->hist_count], CMDSUG_CMDLEN, "%s", chosen);
        r->hist_freq[r->hist_count++] = 1;
        return;
    }
    for (i = 1; i < r->hist_count; i++)                 /* evict least-frequent */
        if (r->hist_freq[i] < r->hist_freq[min_i]) min_i = i;
    snprintf(r->history[min_i], CMDSUG_CMDLEN, "%s", chosen);
    r->hist_freq[min_i] = 1;
}
