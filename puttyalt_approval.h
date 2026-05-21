#ifndef PUTTYALT_APPROVAL_H
#define PUTTYALT_APPROVAL_H

#define APPROVAL_MAX_REQUESTS 64

typedef enum {
    APPROVAL_PENDING,
    APPROVAL_GRANTED,
    APPROVAL_DENIED,
    APPROVAL_EXPIRED
} ApprovalStatus;

typedef struct {
    char requester[64];
    char target_host[128];
    int target_port;
    char reason[256];
    long requested_at;
    long expires_at;
    ApprovalStatus status;
    char approver[64];
    long decided_at;
} ApprovalRequest;

typedef struct {
    ApprovalRequest requests[APPROVAL_MAX_REQUESTS];
    int count;
    int enabled;
    int auto_expire_minutes;
    char required_hosts[512];  /* patterns that need approval */
} ApprovalManager;

int  approval_init(ApprovalManager *am);
int  approval_request(ApprovalManager *am, const char *requester,
                      const char *host, int port, const char *reason);
int  approval_grant(ApprovalManager *am, int idx, const char *approver);
int  approval_deny(ApprovalManager *am, int idx, const char *approver);
int  approval_check(const ApprovalManager *am, const char *host, int port);
void approval_expire_old(ApprovalManager *am);
void approval_destroy(ApprovalManager *am);

#endif
