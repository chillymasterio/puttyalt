/*
 * puttyalt_import.c: Session import from SSH config files.
 */

#include "puttyalt_import.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

const char *import_default_ssh_config_path(void)
{
    static char path[512];
#ifdef _WIN32
    const char *home = getenv("USERPROFILE");
#else
    const char *home = getenv("HOME");
#endif
    if (!home) return NULL;
    snprintf(path, sizeof(path), "%s/.ssh/config", home);
    return path;
}

int import_from_ssh_config(ImportResult *result, const char *path)
{
    FILE *f;
    char line[1024];
    ImportedSession *cur = NULL;

    memset(result, 0, sizeof(*result));

    f = fopen(path, "r");
    if (!f) {
        snprintf(result->error_msg, sizeof(result->error_msg),
                 "Cannot open %s", path);
        return -1;
    }

    while (fgets(line, sizeof(line), f)) {
        size_t len = strlen(line);
        while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r'))
            line[--len] = '\0';

        char *p = line;
        while (*p && isspace((unsigned char)*p)) p++;
        if (!*p || *p == '#') continue;

        if (strncasecmp(p, "Host ", 5) == 0) {
            char *name = p + 5;
            while (*name && isspace((unsigned char)*name)) name++;
            if (strchr(name, '*') || strchr(name, '?'))
                continue;

            if (result->count >= MAX_IMPORT_SESSIONS) break;
            cur = &result->sessions[result->count++];
            memset(cur, 0, sizeof(*cur));
            snprintf(cur->name, sizeof(cur->name), "%s", name);
            snprintf(cur->hostname, sizeof(cur->hostname), "%s", name);
            cur->port = 22;
            cur->protocol = 0;
        } else if (cur) {
            if (strncasecmp(p, "HostName ", 9) == 0) {
                char *val = p + 9;
                while (*val && isspace((unsigned char)*val)) val++;
                snprintf(cur->hostname, sizeof(cur->hostname), "%s", val);
            } else if (strncasecmp(p, "Port ", 5) == 0) {
                cur->port = atoi(p + 5);
            } else if (strncasecmp(p, "User ", 5) == 0) {
                char *val = p + 5;
                while (*val && isspace((unsigned char)*val)) val++;
                snprintf(cur->username, sizeof(cur->username), "%s", val);
            } else if (strncasecmp(p, "IdentityFile ", 13) == 0) {
                char *val = p + 13;
                while (*val && isspace((unsigned char)*val)) val++;
                snprintf(cur->keyfile, sizeof(cur->keyfile), "%s", val);
            }
        }
    }

    fclose(f);
    return result->count;
}

/* Session import from config files only (no registry access) */
int import_from_registry(ImportResult *result)
{
    memset(result, 0, sizeof(*result));
    snprintf(result->error_msg, sizeof(result->error_msg),
             "Use File > Import > SSH Config to import sessions");
    return -1;
}

int import_from_kitty(ImportResult *result, const char *dir)
{
    (void)dir;
    memset(result, 0, sizeof(*result));
    snprintf(result->error_msg, sizeof(result->error_msg),
             "KiTTY import not yet implemented");
    return -1;
}
