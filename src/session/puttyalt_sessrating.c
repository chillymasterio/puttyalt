#include <string.h>
#include <stdio.h>

#define MAX_RATED_SESSIONS 256

typedef struct {
    char name[128];
    int rating;      /* 1-5 stars */
    int use_count;
    long last_used;
} RatedSession;

static RatedSession g_rated[MAX_RATED_SESSIONS];
static int g_rated_count = 0;

int sessrating_set(const char *name, int rating)
{
    if (!name || rating < 1 || rating > 5) return -1;
    for (int i = 0; i < g_rated_count; i++) {
        if (strcmp(g_rated[i].name, name) == 0) {
            g_rated[i].rating = rating;
            return 0;
        }
    }
    if (g_rated_count >= MAX_RATED_SESSIONS) return -1;
    snprintf(g_rated[g_rated_count].name, 128, "%s", name);
    g_rated[g_rated_count].rating = rating;
    g_rated[g_rated_count].use_count = 0;
    g_rated_count++;
    return 0;
}

int sessrating_get(const char *name)
{
    for (int i = 0; i < g_rated_count; i++)
        if (strcmp(g_rated[i].name, name) == 0) return g_rated[i].rating;
    return 0;
}

void sessrating_increment_use(const char *name)
{
    for (int i = 0; i < g_rated_count; i++)
        if (strcmp(g_rated[i].name, name) == 0) { g_rated[i].use_count++; return; }
}

int sessrating_get_top(char names[][128], int max)
{
    int count = 0;
    for (int r = 5; r >= 1 && count < max; r--)
        for (int i = 0; i < g_rated_count && count < max; i++)
            if (g_rated[i].rating == r)
                snprintf(names[count++], 128, "%s", g_rated[i].name);
    return count;
}
