/*
 * puttyalt_import.h: Import sessions from external sources.
 *
 * Supports importing from:
 * - Windows Registry (PuTTY saved sessions)
 * - KiTTY portable sessions
 * - SSH config files (~/.ssh/config)
 * - MobaXterm bookmark exports
 */

#ifndef PUTTYALT_IMPORT_H
#define PUTTYALT_IMPORT_H

#define MAX_IMPORT_SESSIONS 512

typedef struct ImportedSession {
    char name[256];
    char hostname[256];
    int  port;
    char username[128];
    char keyfile[512];
    int  protocol;      /* 0=SSH, 1=Telnet, 2=Raw, 3=Serial */
} ImportedSession;

typedef struct ImportResult {
    ImportedSession sessions[MAX_IMPORT_SESSIONS];
    int count;
    int errors;
    char error_msg[256];
} ImportResult;

/* Import from PuTTY registry (Windows only) */
int import_from_registry(ImportResult *result);

/* Import from SSH config file */
int import_from_ssh_config(ImportResult *result, const char *path);

/* Import from KiTTY sessions directory */
int import_from_kitty(ImportResult *result, const char *dir);

/* Auto-detect default SSH config location */
const char *import_default_ssh_config_path(void);

#endif /* PUTTYALT_IMPORT_H */
