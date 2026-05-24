#ifndef PUTTYALT_MIGRATE_H
#define PUTTYALT_MIGRATE_H

#define MIGRATE_MAX_SESSIONS 128

typedef enum {
    MIGRATE_PUTTY_REG = 0,
    MIGRATE_KITTY,
    MIGRATE_MOBAXTERM,
    MIGRATE_SECURECRT,
    MIGRATE_SSH_CONFIG
} MigrateSource;

typedef struct MigratedSession {
    char name[128];
    char host[256];
    int port;
    char username[128];
    char key_path[512];
    int imported;
} MigratedSession;

typedef struct MigrationResult {
    MigratedSession sessions[MIGRATE_MAX_SESSIONS];
    int count;
    int errors;
    MigrateSource source;
} MigrationResult;

int migrate_scan(MigrationResult *result, MigrateSource source);
int migrate_import_all(MigrationResult *result);
int migrate_import_one(MigrationResult *result, int index);
const char *migrate_source_name(MigrateSource source);

#endif
