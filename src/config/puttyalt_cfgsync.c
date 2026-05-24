#include "puttyalt_cfgsync.h"
#include <string.h>
#include <stdio.h>
#include <time.h>

int cfgsync_init(CfgSyncState *cs)
{
    memset(cs, 0, sizeof(*cs));
    cs->format = 0;  /* JSON default */
    return 0;
}

int cfgsync_export(CfgSyncState *cs, const char *path)
{
    if (!path || !path[0]) return -1;
    FILE *f = fopen(path, "w");
    if (!f) return -1;

    fprintf(f, "{\n");
    fprintf(f, "  \"version\": \"2.0.0\",\n");
    fprintf(f, "  \"exported_at\": %ld,\n", (long)time(NULL));
    fprintf(f, "  \"sections\": [\n");

    for (int i = 0; i < cs->section_count; i++) {
        const char *name = "unknown";
        switch (cs->sections[i]) {
        case CFGSYNC_EXPORT_ALL: name = "all"; break;
        case CFGSYNC_EXPORT_SESSIONS: name = "sessions"; break;
        case CFGSYNC_EXPORT_APPEARANCE: name = "appearance"; break;
        case CFGSYNC_EXPORT_KEYS: name = "keys"; break;
        case CFGSYNC_EXPORT_SNIPPETS: name = "snippets"; break;
        }
        fprintf(f, "    \"%s\"%s\n", name, i < cs->section_count - 1 ? "," : "");
    }

    fprintf(f, "  ],\n");
    fprintf(f, "  \"encrypted\": %s\n", cs->encrypt ? "true" : "false");
    fprintf(f, "}\n");

    fclose(f);
    snprintf(cs->export_path, sizeof(cs->export_path), "%s", path);
    cs->last_export = (long)time(NULL);
    return 0;
}

int cfgsync_import(CfgSyncState *cs, const char *path)
{
    if (!path || !path[0]) return -1;
    FILE *f = fopen(path, "r");
    if (!f) return -1;
    /* Validate JSON structure */
    char buf[4096];
    size_t n = fread(buf, 1, sizeof(buf) - 1, f);
    buf[n] = '\0';
    fclose(f);

    if (!strstr(buf, "\"version\"")) return -2;  /* invalid format */

    snprintf(cs->import_path, sizeof(cs->import_path), "%s", path);
    cs->last_import = (long)time(NULL);
    return 0;
}

int cfgsync_add_section(CfgSyncState *cs, CfgSyncSection section)
{
    if (cs->section_count >= CFGSYNC_MAX_SECTIONS) return -1;
    cs->sections[cs->section_count++] = section;
    return 0;
}

int cfgsync_auto_backup(CfgSyncState *cs, const char *dir)
{
    if (!dir || !dir[0]) return -1;
    char path[1024];
    time_t now = time(NULL);
    struct tm *tm = localtime(&now);
    snprintf(path, sizeof(path), "%s/puttyalt_backup_%04d%02d%02d_%02d%02d.json",
             dir, tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
             tm->tm_hour, tm->tm_min);
    return cfgsync_export(cs, path);
}

void cfgsync_destroy(CfgSyncState *cs)
{
    memset(cs, 0, sizeof(*cs));
}
