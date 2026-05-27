#include <string.h>
#include <stdio.h>
#include <time.h>
#define AUDIT_MAX 1024
typedef enum { AUDIT_CONNECT, AUDIT_DISCONNECT, AUDIT_AUTH_OK, AUDIT_AUTH_FAIL, AUDIT_COMMAND, AUDIT_TRANSFER } AuditType;
typedef struct { AuditType type; char detail[256]; time_t timestamp; char session[128]; } AuditEntry;
static AuditEntry g_audit[AUDIT_MAX]; static int g_audit_count = 0; static FILE *g_audit_fp = NULL;
void audit_init(const char *logfile) {
    if (logfile) g_audit_fp = fopen(logfile, "a");
}
void audit_log(AuditType type, const char *session, const char *detail) {
    if (g_audit_count >= AUDIT_MAX) { memmove(&g_audit[0], &g_audit[1], sizeof(AuditEntry)*(AUDIT_MAX-1)); g_audit_count--; }
    AuditEntry *e = &g_audit[g_audit_count++];
    e->type = type; e->timestamp = time(NULL);
    snprintf(e->session, 128, "%s", session ? session : "");
    snprintf(e->detail, 256, "%s", detail ? detail : "");
    if (g_audit_fp) {
        const char *types[] = {"CONNECT","DISCONNECT","AUTH_OK","AUTH_FAIL","COMMAND","TRANSFER"};
        fprintf(g_audit_fp, "[%ld] %s %s %s\n", (long)e->timestamp, types[type], e->session, e->detail);
        fflush(g_audit_fp);
    }
}
int audit_count(void) { return g_audit_count; }
void audit_close(void) { if (g_audit_fp) { fclose(g_audit_fp); g_audit_fp = NULL; } }
