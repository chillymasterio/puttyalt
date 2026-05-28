#include <string.h>
#include <stdio.h>
typedef struct { char query[256]; int case_sensitive; int regex; int wrap; int visible; int match_count; int current_match; } SearchBar;
static SearchBar g_sb = {"", 0, 0, 1, 0, 0, 0};
void searchbar_show(void) { g_sb.visible = 1; }
void searchbar_hide(void) { g_sb.visible = 0; g_sb.query[0] = 0; g_sb.match_count = 0; }
void searchbar_set_query(const char *q) { snprintf(g_sb.query, 256, "%s", q); }
void searchbar_toggle_case(void) { g_sb.case_sensitive = !g_sb.case_sensitive; }
void searchbar_toggle_regex(void) { g_sb.regex = !g_sb.regex; }
void searchbar_set_results(int count) { g_sb.match_count = count; g_sb.current_match = count > 0 ? 1 : 0; }
void searchbar_next(void) { if (g_sb.match_count > 0) g_sb.current_match = g_sb.current_match % g_sb.match_count + 1; }
void searchbar_prev(void) { if (g_sb.match_count > 0) g_sb.current_match = (g_sb.current_match - 2 + g_sb.match_count) % g_sb.match_count + 1; }
int searchbar_format(char *buf, int buflen) {
    if (!g_sb.visible) return 0;
    return snprintf(buf, buflen, "Find: %s [%d/%d]%s%s", g_sb.query, g_sb.current_match, g_sb.match_count, g_sb.case_sensitive?" Aa":"", g_sb.regex?" .*":"");
}
int searchbar_visible(void) { return g_sb.visible; }
