/* puttyalt_settingvalidate.c - Setting value validation with type coercion. */
#include <string.h>
#include <stdlib.h>
enum sv_type { SV_STRING = 0, SV_INT = 1, SV_BOOL = 2, SV_ENUM = 3, SV_COLOR = 4 };
int settingvalidate_int(const char *value, int min, int max, int *out) {
    if (!value) return -1;
    for (const char *p = value; *p; p++) {
        if ((*p < '0' || *p > '9') && *p != '-') return -1;
    }
    int v = atoi(value);
    if (v < min || v > max) return -1;
    if (out) *out = v;
    return 0;
}
int settingvalidate_bool(const char *value, int *out) {
    if (!value) return -1;
    if (!strcmp(value, "1") || !strcmp(value, "true") || !strcmp(value, "yes") || !strcmp(value, "on")) {
        if (out) *out = 1;
        return 0;
    }
    if (!strcmp(value, "0") || !strcmp(value, "false") || !strcmp(value, "no") || !strcmp(value, "off")) {
        if (out) *out = 0;
        return 0;
    }
    return -1;
}
int settingvalidate_color(const char *value, unsigned int *out) {
    if (!value || value[0] != '#') return -1;
    int len = (int)strlen(value + 1);
    if (len != 6 && len != 8) return -1;
    for (const char *p = value + 1; *p; p++) {
        if (!((*p >= '0' && *p <= '9') || (*p >= 'a' && *p <= 'f') || (*p >= 'A' && *p <= 'F'))) return -1;
    }
    if (out) *out = (unsigned int)strtol(value + 1, NULL, 16);
    return 0;
}
int settingvalidate_enum(const char *value, const char **options, int n) {
    if (!value || !options) return -1;
    for (int i = 0; i < n; i++) {
        if (strcmp(value, options[i]) == 0) return i;
    }
    return -1;
}
