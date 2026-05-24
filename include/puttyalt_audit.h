/*
 * puttyalt_audit.h — Session audit trail for compliance.
 * v1.4.0 feature: log all connection events and commands.
 */

#include <stdio.h>
#ifndef PUTTYALT_AUDIT_H
#define PUTTYALT_AUDIT_H

#define AUDIT_MAX_ENTRIES   10000
#define AUDIT_MAX_MSG       256
#define AUDIT_MAX_USER      64
#define AUDIT_MAX_HOST      128

typedef enum {
    AUDIT_CONNECT,
    AUDIT_DISCONNECT,
    AUDIT_AUTH_SUCCESS,
    AUDIT_AUTH_FAIL,
    AUDIT_COMMAND,
    AUDIT_FILE_TRANSFER,
    AUDIT_PORT_FORWARD,
    AUDIT_CONFIG_CHANGE,
    AUDIT_SESSION_LOCK,
    AUDIT_SESSION_UNLOCK
} AuditEventType;

typedef struct {
    AuditEventType type;
    long timestamp;
    char user[AUDIT_MAX_USER];
    char host[AUDIT_MAX_HOST];
    int port;
    char message[AUDIT_MAX_MSG];
    unsigned long session_id;
} AuditEntry;

typedef struct {
    AuditEntry *entries;
    int count;
    int capacity;
    int enabled;
    int log_commands;
    int log_transfers;
    char log_path[512];
    FILE *log_file;
    unsigned long next_session_id;
} AuditTrail;

int  audit_init(AuditTrail *at, const char *log_path);
void audit_destroy(AuditTrail *at);
int  audit_log(AuditTrail *at, AuditEventType type,
               const char *user, const char *host, int port,
               const char *message);
int  audit_set_session(AuditTrail *at, unsigned long session_id);
int  audit_flush(AuditTrail *at);
int  audit_export_csv(const AuditTrail *at, const char *path);
int  audit_search(const AuditTrail *at, const char *query,
                  AuditEntry *results, int max_results);
int  audit_rotate_log(AuditTrail *at, long max_size_bytes);
int  audit_clear_before(AuditTrail *at, long timestamp);

#endif /* PUTTYALT_AUDIT_H */
