#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX_ENVS 128
typedef struct { char key[128]; char value[512]; } EnvVar;
static EnvVar g_envs[MAX_ENVS]; static int g_env_count = 0;
int envvar_set(const char *key, const char *value) {
    for (int i = 0; i < g_env_count; i++)
        if (strcmp(g_envs[i].key, key) == 0) { snprintf(g_envs[i].value, 512, "%s", value); return 0; }
    if (g_env_count >= MAX_ENVS) return -1;
    snprintf(g_envs[g_env_count].key, 128, "%s", key);
    snprintf(g_envs[g_env_count].value, 512, "%s", value);
    g_env_count++; return 0;
}
const char *envvar_get(const char *key) {
    for (int i = 0; i < g_env_count; i++) if (strcmp(g_envs[i].key, key) == 0) return g_envs[i].value;
    return getenv(key);
}
int envvar_expand(const char *input, char *output, int outlen) {
    int pos = 0;
    for (const char *p = input; *p && pos < outlen - 1; p++) {
        if (*p == '$' && *(p+1) == '{') {
            char key[128]; int ki = 0; p += 2;
            while (*p && *p != '}' && ki < 127) key[ki++] = *p++;
            key[ki] = 0;
            const char *val = envvar_get(key);
            if (val) pos += snprintf(output + pos, outlen - pos, "%s", val);
        } else output[pos++] = *p;
    }
    output[pos] = 0; return pos;
}
