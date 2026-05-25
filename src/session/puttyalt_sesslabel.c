#include <string.h>
#include <stdio.h>

#define MAX_LABELS 128
#define LABEL_LEN 32

typedef struct { char name[128]; char labels[8][LABEL_LEN]; int label_count; } LabeledSession;
static LabeledSession g_labeled[MAX_LABELS];
static int g_labeled_count = 0;

int sesslabel_add(const char *session, const char *label)
{
    for (int i = 0; i < g_labeled_count; i++) {
        if (strcmp(g_labeled[i].name, session) == 0) {
            if (g_labeled[i].label_count >= 8) return -1;
            snprintf(g_labeled[i].labels[g_labeled[i].label_count++], LABEL_LEN, "%s", label);
            return 0;
        }
    }
    if (g_labeled_count >= MAX_LABELS) return -1;
    LabeledSession *s = &g_labeled[g_labeled_count++];
    snprintf(s->name, 128, "%s", session);
    snprintf(s->labels[0], LABEL_LEN, "%s", label);
    s->label_count = 1;
    return 0;
}

int sesslabel_find_by_label(const char *label, char names[][128], int max)
{
    int n = 0;
    for (int i = 0; i < g_labeled_count && n < max; i++)
        for (int j = 0; j < g_labeled[i].label_count; j++)
            if (strcmp(g_labeled[i].labels[j], label) == 0) {
                snprintf(names[n++], 128, "%s", g_labeled[i].name);
                break;
            }
    return n;
}
