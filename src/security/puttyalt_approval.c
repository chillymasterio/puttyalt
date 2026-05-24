#include "puttyalt_approval.h"
#include <string.h>
#include <stdio.h>
#include <time.h>

int approval_init(ApprovalManager *am)
{
    memset(am, 0, sizeof(*am));
    am->auto_expire_minutes = 60;
    return 0;
}

int approval_request(ApprovalManager *am, const char *requester,
                     const char *host, int port, const char *reason)
{
    if (am->count >= APPROVAL_MAX_REQUESTS) return -1;
    ApprovalRequest *r = &am->requests[am->count];
    memset(r, 0, sizeof(*r));
    snprintf(r->requester, sizeof(r->requester), "%s", requester);
    snprintf(r->target_host, sizeof(r->target_host), "%s", host);
    r->target_port = port;
    if (reason) snprintf(r->reason, sizeof(r->reason), "%s", reason);
    r->requested_at = (long)time(NULL);
    r->expires_at = r->requested_at + am->auto_expire_minutes * 60;
    r->status = APPROVAL_PENDING;
    am->count++;
    return am->count - 1;
}

int approval_grant(ApprovalManager *am, int idx, const char *approver)
{
    if (idx < 0 || idx >= am->count) return -1;
    am->requests[idx].status = APPROVAL_GRANTED;
    am->requests[idx].decided_at = (long)time(NULL);
    if (approver)
        snprintf(am->requests[idx].approver, 64, "%s", approver);
    return 0;
}

int approval_deny(ApprovalManager *am, int idx, const char *approver)
{
    if (idx < 0 || idx >= am->count) return -1;
    am->requests[idx].status = APPROVAL_DENIED;
    am->requests[idx].decided_at = (long)time(NULL);
    if (approver)
        snprintf(am->requests[idx].approver, 64, "%s", approver);
    return 0;
}

int approval_check(const ApprovalManager *am, const char *host, int port)
{
    if (!am->enabled) return 1;  /* approved by default when disabled */
    long now = (long)time(NULL);
    for (int i = am->count - 1; i >= 0; i--) {
        const ApprovalRequest *r = &am->requests[i];
        if (r->status != APPROVAL_GRANTED) continue;
        if (now > r->expires_at) continue;
        if (strcmp(r->target_host, host) == 0 && r->target_port == port)
            return 1;  /* approved */
    }
    return 0;  /* not approved */
}

void approval_expire_old(ApprovalManager *am)
{
    long now = (long)time(NULL);
    for (int i = 0; i < am->count; i++) {
        if (am->requests[i].status == APPROVAL_PENDING &&
            now > am->requests[i].expires_at)
            am->requests[i].status = APPROVAL_EXPIRED;
    }
}

void approval_destroy(ApprovalManager *am) { memset(am, 0, sizeof(*am)); }
