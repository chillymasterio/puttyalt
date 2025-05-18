#include <string.h>
#include <stdio.h>
#include "puttyalt_migrate.h"

const char *migrate_source_name(MigrateSource source)
{
    switch (source) {
        case MIGRATE_PUTTY_REG:  return "PuTTY Registry";
        case MIGRATE_KITTY:      return "KiTTY";
        case MIGRATE_MOBAXTERM:   return "MobaXterm";
        case MIGRATE_SECURECRT:   return "SecureCRT";
        case MIGRATE_SSH_CONFIG:  return "SSH Config";
    }
    return "Unknown";
}

int migrate_scan(MigrationResult *result, MigrateSource source)
{
    memset(result, 0, sizeof(*result));
    result->source = source;

    switch (source) {
    case MIGRATE_SSH_CONFIG: {
        const char *home = getenv("HOME");
        if (!home) home = getenv("USERPROFILE");
        if (!home) return -1;

        char path[512];
        snprintf(path, sizeof(path), "%s/.ssh/config", home);
        FILE *fp = fopen(path, "r");
        if (!fp) return -1;

        char line[1024];
        MigratedSession *cur = NULL;
        while (fgets(line, sizeof(line), fp)) {
            size_t len = strlen(line);
            while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r'))
                line[--len] = '\0';

            if (strncasecmp(line, "Host ", 5) == 0 && line[5] != '*') {
                if (result->count >= MIGRATE_MAX_SESSIONS) break;
                cur = &result->sessions[result->count++];
                memset(cur, 0, sizeof(*cur));
                strncpy(cur->name, line + 5, sizeof(cur->name) - 1);
                strncpy(cur->host, line + 5, sizeof(cur->host) - 1);
                cur->port = 22;
            } else if (cur) {
                /* Skip leading whitespace */
                const char *p = line;
                while (*p == ' ' || *p == '\t') p++;

                if (strncasecmp(p, "HostName ", 9) == 0)
                    strncpy(cur->host, p + 9, sizeof(cur->host) - 1);
                else if (strncasecmp(p, "Port ", 5) == 0)
                    cur->port = atoi(p + 5);
                else if (strncasecmp(p, "User ", 5) == 0)
                    strncpy(cur->username, p + 5, sizeof(cur->username) - 1);
                else if (strncasecmp(p, "IdentityFile ", 13) == 0)
                    strncpy(cur->key_path, p + 13, sizeof(cur->key_path) - 1);
            }
        }
        fclose(fp);
        break;
    }

#ifdef _WIN32
    case MIGRATE_PUTTY_REG:
        /* Would enumerate HKCU\Software\SimonTatham\PuTTY\Sessions */
        break;
#endif

    default:
        return -1;
    }

    return result->count;
}

int migrate_import_all(MigrationResult *result)
{
    int imported = 0;
    for (int i = 0; i < result->count; i++) {
        if (!result->sessions[i].imported) {
            result->sessions[i].imported = 1;
            imported++;
        }
    }
    return imported;
}

int migrate_import_one(MigrationResult *result, int index)
{
    if (index < 0 || index >= result->count) return -1;
    result->sessions[index].imported = 1;
    return 0;
}
