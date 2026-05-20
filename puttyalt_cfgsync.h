#ifndef PUTTYALT_CFGSYNC_H
#define PUTTYALT_CFGSYNC_H

#define CFGSYNC_MAX_SECTIONS 32

typedef enum {
    CFGSYNC_EXPORT_ALL,
    CFGSYNC_EXPORT_SESSIONS,
    CFGSYNC_EXPORT_APPEARANCE,
    CFGSYNC_EXPORT_KEYS,
    CFGSYNC_EXPORT_SNIPPETS
} CfgSyncSection;

typedef struct {
    int sections[CFGSYNC_MAX_SECTIONS];
    int section_count;
    char export_path[512];
    char import_path[512];
    int encrypt;
    int include_passwords;
    int format;  /* 0=json, 1=ini, 2=binary */
    long last_export;
    long last_import;
} CfgSyncState;

int  cfgsync_init(CfgSyncState *cs);
int  cfgsync_export(CfgSyncState *cs, const char *path);
int  cfgsync_import(CfgSyncState *cs, const char *path);
int  cfgsync_add_section(CfgSyncState *cs, CfgSyncSection section);
int  cfgsync_auto_backup(CfgSyncState *cs, const char *dir);
void cfgsync_destroy(CfgSyncState *cs);

#endif
