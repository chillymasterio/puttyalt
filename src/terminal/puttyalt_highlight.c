#include <string.h>
#include <stdio.h>
#define MAX_RULES 32
typedef struct { char pattern[256]; unsigned int fg; unsigned int bg; int bold; int enabled; } HighlightRule;
static HighlightRule g_rules[MAX_RULES]; static int g_rule_count = 0;
int highlight_add(const char *pattern, unsigned int fg, unsigned int bg, int bold) {
    if (g_rule_count >= MAX_RULES) return -1;
    HighlightRule *r = &g_rules[g_rule_count]; memset(r, 0, sizeof(*r));
    snprintf(r->pattern, 256, "%s", pattern); r->fg = fg; r->bg = bg; r->bold = bold; r->enabled = 1;
    return g_rule_count++;
}
int highlight_match(const char *text, int *rule_idx) {
    for (int i = 0; i < g_rule_count; i++)
        if (g_rules[i].enabled && strstr(text, g_rules[i].pattern)) { *rule_idx = i; return 1; }
    return 0;
}
void highlight_init_defaults(void) {
    g_rule_count = 0;
    highlight_add("error", 0xff5555, 0, 1);
    highlight_add("ERROR", 0xff5555, 0, 1);
    highlight_add("warning", 0xffaa00, 0, 0);
    highlight_add("WARNING", 0xffaa00, 0, 0);
    highlight_add("success", 0x55ff55, 0, 0);
    highlight_add("OK", 0x55ff55, 0, 1);
    highlight_add("FAIL", 0xff5555, 0x330000, 1);
}
int highlight_count(void) { return g_rule_count; }
