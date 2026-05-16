/*
 * puttyalt_auditlog.h: Security audit logging.
 *
 * Logs connection attempts, authentication events, and file transfers
 * to a tamper-evident audit log for compliance.
 */

#ifndef PUTTYALT_AUDITLOG_H
#define PUTTYALT_AUDITLOG_H

typedef enum {
    AUDIT_CONNECT,
    AUDIT_DISCONNECT,
    AUDIT_AUTH_SUCCESS,
    AUDIT_AUTH_FAILURE,
    AUDIT_FILE_UPLOAD,
    AUDIT_FILE_DOWNLOAD,
    AUDIT_KEY_FORWARDED,
    AUDIT_COMMAND_EXEC
} AuditEventType;

typedef struct AuditEvent {
    AuditEventType type;
    long           timestamp;
    char           session[128];
    char           username[128];
    char           hostname[256];
    int            port;
    char           detail[512];
} AuditEvent;

int audit_log_init(const char *log_path);
int audit_log_event(const AuditEvent *event);
void audit_log_close(void);

#endif /* PUTTYALT_AUDITLOG_H */
