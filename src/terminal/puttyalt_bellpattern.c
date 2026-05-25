#include <string.h>
#include <stdio.h>

#define MAX_PATTERNS 16
#define PATTERN_NAME_LEN 32

typedef struct {
    char name[PATTERN_NAME_LEN];
    int frequency;
    int duration_ms;
    int repeat;
    int visual_flash;
} BellPattern;

static BellPattern g_patterns[MAX_PATTERNS];
static int g_pattern_count = 0;
static int g_active_pattern = 0;

void bellpattern_init(void)
{
    g_pattern_count = 0;
    struct { const char *n; int f; int d; int r; int v; } presets[] = {
        {"Silent",     0,    0,   0, 0},
        {"Short Beep", 800,  50,  1, 0},
        {"Double Tap", 600,  30,  2, 0},
        {"Alert",      1000, 100, 3, 1},
        {"Visual Only",0,    0,   0, 1},
        {"Subtle",     400,  20,  1, 0},
    };
    for (int i = 0; i < 6; i++) {
        BellPattern *p = &g_patterns[g_pattern_count++];
        snprintf(p->name, PATTERN_NAME_LEN, "%s", presets[i].n);
        p->frequency = presets[i].f;
        p->duration_ms = presets[i].d;
        p->repeat = presets[i].r;
        p->visual_flash = presets[i].v;
    }
    g_active_pattern = 1;
}

int bellpattern_set(const char *name)
{
    for (int i = 0; i < g_pattern_count; i++)
        if (strcmp(g_patterns[i].name, name) == 0) { g_active_pattern = i; return 0; }
    return -1;
}

BellPattern *bellpattern_active(void)
{
    return &g_patterns[g_active_pattern];
}
