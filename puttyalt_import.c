/*
 * puttyalt_import.c: Session import implementation.
 */

#include "puttyalt_import.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#ifdef _WIN32
#include <windows.h>
#endif

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

        /* Skip comments and empty lines */
        char *p = line;
        while (*p && isspace((unsigned char)*p)) p++;
        if (!*p || *p == '#') continue;

        if (strncasecmp(p, "Host ", 5) == 0) {
            char *name = p + 5;
            while (*name && isspace((unsigned char)*name)) name++;
            /* Skip wildcard-only entries */
            if (strchr(name, '*') || strchr(name, '?'))
                continue;

            if (result->count >= MAX_IMPORT_SESSIONS) break;
            cur = &result->sessions[result->count++];
            memset(cur, 0, sizeof(*cur));
            snprintf(cur->name, sizeof(cur->name), "%s", name);
            snprintf(cur->hostname, sizeof(cur->hostname), "%s", name);
            cur->port = 22;
            cur->protocol = 0; /* SSH */
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

#ifdef _WIN32
int import_from_registry(ImportResult *result)
{
    HKEY sessions_key;
    LONG ret;
    DWORD idx = 0;
    char name[256];
    DWORD name_len;

    memset(result, 0, sizeof(*result));

    ret = RegOpenKeyExA(HKEY_CURRENT_USER,
                        "Software\\SimonTatham\\PuTTY\\Sessions",
                        0, KEY_READ, &sessions_key);
    if (ret != ERROR_SUCCESS) {
        snprintf(result->error_msg, sizeof(result->error_msg),
                 "Cannot open PuTTY registry key");
        return -1;
    }

    while (result->count < MAX_IMPORT_SESSIONS) {
        name_len = sizeof(name);
        ret = RegEnumKeyExA(sessions_key, idx++, name, &name_len,
                           NULL, NULL, NULL, NULL);
        if (ret != ERROR_SUCCESS) break;

        HKEY session_key;
        char subkey[512];
        snprintf(subkey, sizeof(subkey),
                 "Software\\SimonTatham\\PuTTY\\Sessions\\%s", name);

        if (RegOpenKeyExA(HKEY_CURRENT_USER, subkey, 0, KEY_READ,
                         &session_key) == ERROR_SUCCESS) {
            ImportedSession *s = &result->sessions[result->count];
            memset(s, 0, sizeof(*s));

            /* URL-decode the session name */
            snprintf(s->name, sizeof(s->name), "%s", name);

            char hostname[256] = {0};
            DWORD hlen = sizeof(hostname);
            RegQueryValueExA(session_key, "HostName", NULL, NULL,
                           (LPBYTE)hostname, &hlen);
            snprintf(s->hostname, sizeof(s->hostname), "%s", hostname);

            DWORD port = 22;
            DWORD plen = sizeof(port);
            RegQueryValueExA(session_key, "PortNumber", NULL, NULL,
                           (LPBYTE)&port, &plen);
            s->port = (int)port;

            DWORD proto = 0;
            DWORD prlen = sizeof(proto);
            RegQueryValueExA(session_key, "Protocol", NULL, NULL,
                           (LPBYTE)&proto, &prlen);
            s->protocol = (int)proto;

            RegCloseKey(session_key);

            if (s->hostname[0])
                result->count++;
        }
    }

    RegCloseKey(sessions_key);
    return result->count;
}
#else
int import_from_registry(ImportResult *result)
{
    memset(result, 0, sizeof(*result));
    snprintf(result->error_msg, sizeof(result->error_msg),
             "Registry import only available on Windows");
    return -1;
}
#endif

int import_from_kitty(ImportResult *result, const char *dir)
{
    (void)dir;
    memset(result, 0, sizeof(*result));
    snprintf(result->error_msg, sizeof(result->error_msg),
             "KiTTY import not yet implemented");
    return -1;
}
