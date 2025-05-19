#include <string.h>
#include <ctype.h>
#include "puttyalt_validate.h"

void val_init(ValidationResult *vr) { memset(vr, 0, sizeof(*vr)); }

void val_add(ValidationResult *vr, ValidationLevel level,
             const char *field, const char *msg)
{
    if (vr->count >= 32) return;
    ValidationMessage *m = &vr->messages[vr->count++];
    m->level = level;
    strncpy(m->field, field, sizeof(m->field) - 1);
    strncpy(m->message, msg, sizeof(m->message) - 1);
    if (level == VAL_ERROR) vr->has_errors = 1;
}

int val_port(int port) { return port >= 1 && port <= 65535; }

int val_hostname(const char *host)
{
    if (!host || strlen(host) == 0 || strlen(host) > 253) return 0;
    for (const char *p = host; *p; p++) {
        if (!isalnum((unsigned char)*p) && *p != '.' && *p != '-' && *p != ':')
            return 0;
    }
    return 1;
}

int val_username(const char *user)
{
    if (!user || strlen(user) == 0 || strlen(user) > 128) return 0;
    /* No spaces or control chars */
    for (const char *p = user; *p; p++)
        if (iscntrl((unsigned char)*p) || *p == ' ') return 0;
    return 1;
}

int val_path(const char *path)
{
    if (!path || strlen(path) == 0) return 0;
    return strlen(path) < 4096;
}
