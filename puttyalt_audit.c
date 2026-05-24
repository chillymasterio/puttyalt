/*
 * puttyalt_audit.c — Session audit trail for compliance.
 */

#include "puttyalt_audit.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static const char *event_names[] = {
    "CONNECT", "DISCONNECT", "AUTH_OK", "AUTH_FAIL",
    "COMMAND", "FILE_XFER", "PORT_FWD", "CONFIG",
    "LOCK", "UNLOCK"
};

int audit_init(AuditTrail *at, const char *log_path)
{
    memset(at, 0, sizeof(*at));
    at->capacity = 1024;
    at->entries = (AuditEntry *)calloc(at->capacity, sizeof(AuditEntry));
    if (!at->entries) return -1;

    at->enabled = 1;
    at->log_commands = 1;
    at->log_transfers = 1;
    at->next_session_id = 1;

    if (log_path && log_path[0]) {
        snprintf(at->log_path, sizeof(at->log_path), "%s", log_path);
        at->log_file = fopen(log_path, "a");
        if (at->log_file) {
            fprintf(at->log_file, "# PuttyAlt Audit Trail — started %ld\n",
                    (long)time(NULL));
            fflush(at->log_file);
        }
    }

    return 0;
}

void audit_destroy(AuditTrail *at)
{
    if (at->log_file) {
        fprintf(at->log_file, "# Audit trail closed %ld\n", (long)time(NULL));
        fclose(at->log_file);
    }
    free(at->entries);
    memset(at, 0, sizeof(*at));
}

int audit_log(AuditTrail *at, AuditEventType type,
              const char *user, const char *host, int port,
              const char *message)
{
    if (!at->enabled) return 0;
    if (!at->log_commands && type == AUDIT_COMMAND) return 0;
    if (!at->log_transfers && type == AUDIT_FILE_TRANSFER) return 0;

    /* Grow buffer if needed */
    if (at->count >= at->capacity) {
        int new_cap = at->capacity * 2;
        if (new_cap > AUDIT_MAX_ENTRIES) new_cap = AUDIT_MAX_ENTRIES;
        if (at->count >= new_cap) {
            /* Rotate: discard oldest 25% */
            int discard = new_cap / 4;
            memmove(at->entries, at->entries + discard,
                    (at->count - discard) * sizeof(AuditEntry));
            at->count -= discard;
        } else {
            AuditEntry *new_buf = (AuditEntry *)realloc(
                at->entries, new_cap * sizeof(AuditEntry));
            if (!new_buf) return -1;
            at->entries = new_buf;
            at->capacity = new_cap;
        }
    }

    AuditEntry *e = &at->entries[at->count];
    memset(e, 0, sizeof(*e));
    e->type = type;
    e->timestamp = (long)time(NULL);
    e->port = port;
    e->session_id = at->next_session_id;

    if (user) snprintf(e->user, sizeof(e->user), "%s", user);
    if (host) snprintf(e->host, sizeof(e->host), "%s", host);
    if (message) snprintf(e->message, sizeof(e->message), "%s", message);

    at->count++;

    /* Write to log file immediately */
    if (at->log_file) {
        const char *etype = (type >= 0 && type <= 9) ?
            event_names[type] : "UNKNOWN";
        fprintf(at->log_file, "%ld\t%s\t%s\t%s:%d\t%s\n",
                e->timestamp, etype,
                e->user[0] ? e->user : "-",
                e->host[0] ? e->host : "-", port,
                e->message[0] ? e->message : "-");
        fflush(at->log_file);
    }

    return 0;
}

int audit_set_session(AuditTrail *at, unsigned long session_id)
{
    at->next_session_id = session_id;
    return 0;
}

int audit_flush(AuditTrail *at)
{
    if (at->log_file) fflush(at->log_file);
    return 0;
}

int audit_export_csv(const AuditTrail *at, const char *path)
{
    FILE *f = fopen(path, "w");
    if (!f) return -1;

    fprintf(f, "timestamp,type,user,host,port,session_id,message\n");
    for (int i = 0; i < at->count; i++) {
        const AuditEntry *e = &at->entries[i];
        const char *etype = (e->type >= 0 && e->type <= 9) ?
            event_names[e->type] : "UNKNOWN";
        fprintf(f, "%ld,%s,%s,%s,%d,%lu,\"%s\"\n",
                e->timestamp, etype, e->user, e->host,
                e->port, e->session_id, e->message);
    }

    fclose(f);
    return 0;
}

int audit_search(const AuditTrail *at, const char *query,
                 AuditEntry *results, int max_results)
{
    if (!query || !query[0]) return 0;
    int found = 0;

    for (int i = 0; i < at->count && found < max_results; i++) {
        const AuditEntry *e = &at->entries[i];
        if (strstr(e->user, query) || strstr(e->host, query) ||
            strstr(e->message, query)) {
            results[found++] = *e;
        }
    }

    return found;
}

int audit_rotate_log(AuditTrail *at, long max_size_bytes)
{
    if (!at) return -1;
    if (!at->log_file || !at->log_path[0]) return 0; /* no file, nothing to rotate */

    /* Check current file size */
    long pos = ftell(at->log_file);
    if (pos < 0 || pos < max_size_bytes) return 0; /* no rotation needed */

    /* Close current log */
    fclose(at->log_file);
    at->log_file = NULL;

    /* Rename current log to .1 (simple single-file rotation) */
    char rotated_path[560];
    snprintf(rotated_path, sizeof(rotated_path), "%s.1", at->log_path);
    rename(at->log_path, rotated_path);

    /* Open fresh log file */
    at->log_file = fopen(at->log_path, "a");
    if (at->log_file) {
        fprintf(at->log_file, "# PuttyAlt Audit Trail — rotated %ld\n",
                (long)time(NULL));
        fflush(at->log_file);
    }
    return 1; /* rotation performed */
}

int audit_clear_before(AuditTrail *at, long timestamp)
{
    int keep_start = 0;
    for (int i = 0; i < at->count; i++) {
        if (at->entries[i].timestamp >= timestamp) {
            keep_start = i;
            break;
        }
        if (i == at->count - 1) keep_start = at->count;
    }

    if (keep_start > 0) {
        int remaining = at->count - keep_start;
        memmove(at->entries, at->entries + keep_start,
                remaining * sizeof(AuditEntry));
        at->count = remaining;
    }
    return at->count;
}
