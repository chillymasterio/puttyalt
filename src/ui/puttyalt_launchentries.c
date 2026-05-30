/* puttyalt_launchentries.c - quick-launch palette entries for PuttyAlt.
 *
 * Holds registered quick-launch command entries and filters them by a
 * typed prefix for palette display. Self-contained library module.
 */

#include <string.h>
#include <stdio.h>

#define LENTRY_MAX        64
#define LENTRY_LABEL_LEN  64
#define LENTRY_ACTION_LEN 256
#define LENTRY_KEYHINT_LEN 24

typedef struct LaunchEntry {
    char label[LENTRY_LABEL_LEN];
    char action[LENTRY_ACTION_LEN];
    char keyhint[LENTRY_KEYHINT_LEN];
} LaunchEntry;

typedef struct LaunchEntryList {
    LaunchEntry items[LENTRY_MAX];
    int count;
} LaunchEntryList;

/* Case-insensitive check that prefix matches the start of s. */
static int lentry_has_prefix(const char *s, const char *prefix)
{
    size_t i;
    for (i = 0; prefix[i] != '\0'; i++) {
        unsigned char a = (unsigned char)s[i];
        unsigned char b = (unsigned char)prefix[i];
        if (a >= 'A' && a <= 'Z') a = (unsigned char)(a - 'A' + 'a');
        if (b >= 'A' && b <= 'Z') b = (unsigned char)(b - 'A' + 'a');
        if (a != b) return 0;
    }
    return 1;
}

void lentry_init(LaunchEntryList *l)
{
    if (l == NULL) return;
    l->count = 0;
    memset(l->items, 0, sizeof(l->items));
}

int lentry_add(LaunchEntryList *l, const char *label,
               const char *action, const char *keyhint)
{
    LaunchEntry *e;
    if (l == NULL || label == NULL || action == NULL) return -1;
    if (label[0] == '\0') return -1;
    if (l->count >= LENTRY_MAX) return -1;

    e = &l->items[l->count];
    snprintf(e->label, sizeof(e->label), "%s", label);
    snprintf(e->action, sizeof(e->action), "%s", action);
    snprintf(e->keyhint, sizeof(e->keyhint), "%s",
             keyhint != NULL ? keyhint : "");
    l->count++;
    return 0;
}

int lentry_filter(const LaunchEntryList *l, const char *query,
                  int *out_idx, int cap)
{
    int i, n = 0;
    if (l == NULL || out_idx == NULL || cap <= 0) return -1;
    for (i = 0; i < l->count && n < cap; i++) {
        if (query == NULL || query[0] == '\0' ||
            lentry_has_prefix(l->items[i].label, query)) {
            out_idx[n++] = i;
        }
    }
    return n;
}

const char *lentry_action_at(const LaunchEntryList *l, int idx)
{
    if (l == NULL || idx < 0 || idx >= l->count) return NULL;
    return l->items[idx].action;
}

int lentry_remove(LaunchEntryList *l, const char *label)
{
    int i, j;
    if (l == NULL || label == NULL) return -1;
    for (i = 0; i < l->count; i++) {
        if (strcmp(l->items[i].label, label) == 0) {
            for (j = i; j < l->count - 1; j++)
                l->items[j] = l->items[j + 1];
            l->count--;
            memset(&l->items[l->count], 0, sizeof(l->items[l->count]));
            return 0;
        }
    }
    return -1;
}

void lentry_clear(LaunchEntryList *l)
{
    if (l == NULL) return;
    l->count = 0;
    memset(l->items, 0, sizeof(l->items));
}
