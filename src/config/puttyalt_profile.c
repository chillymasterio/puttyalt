#include <string.h>
#include <stdio.h>
#define MAX_PROFILES 16
#define PROFILE_NAME_LEN 64
typedef struct { char name[PROFILE_NAME_LEN]; char font[64]; int font_size; char color_scheme[64]; int scrollback; char shell[256]; } Profile;
static Profile g_profiles[MAX_PROFILES]; static int g_profile_count = 0; static int g_active = 0;
int profile_create(const char *name) {
    if (g_profile_count >= MAX_PROFILES) return -1;
    Profile *p = &g_profiles[g_profile_count]; memset(p, 0, sizeof(*p));
    snprintf(p->name, PROFILE_NAME_LEN, "%s", name); p->font_size = 11; p->scrollback = 10000;
    snprintf(p->font, 64, "Consolas"); snprintf(p->color_scheme, 64, "Warm Blue");
    return g_profile_count++;
}
int profile_switch(const char *name) {
    for (int i = 0; i < g_profile_count; i++) if (strcmp(g_profiles[i].name, name) == 0) { g_active = i; return 0; }
    return -1;
}
Profile *profile_active(void) { return g_profile_count > 0 ? &g_profiles[g_active] : NULL; }
int profile_count(void) { return g_profile_count; }
