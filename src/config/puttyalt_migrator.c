#include <string.h>
#include <stdio.h>
typedef struct { int from_version; int to_version; int (*migrate)(void *data); } Migration;
#define MAX_MIGRATIONS 32
static Migration g_migrations[MAX_MIGRATIONS]; static int g_migration_count = 0;
int migrator_register(int from, int to, int (*fn)(void*)) {
    if (g_migration_count >= MAX_MIGRATIONS) return -1;
    g_migrations[g_migration_count].from_version = from;
    g_migrations[g_migration_count].to_version = to;
    g_migrations[g_migration_count].migrate = fn;
    g_migration_count++; return 0;
}
int migrator_run(int current_version, int target_version, void *data) {
    int ver = current_version;
    while (ver < target_version) {
        int found = 0;
        for (int i = 0; i < g_migration_count; i++)
            if (g_migrations[i].from_version == ver) {
                if (g_migrations[i].migrate(data) != 0) return -1;
                ver = g_migrations[i].to_version; found = 1; break;
            }
        if (!found) return -1;
    }
    return 0;
}
