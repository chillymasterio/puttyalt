/* puttyalt_approval.c - Command approval workflow for privileged operations. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define CMDAP_MAX 32
#define CMDAP_CMD 160
enum cmdap_state { CMDAP_PENDING=0, CMDAP_APPROVED=1, CMDAP_DENIED=2, CMDAP_EXPIRED=3 };
typedef struct { char command[CMDAP_CMD]; char requester[40]; int state; uint64_t requested_ms; int timeout_ms; char approver[40]; } cmdap_req;
typedef struct { cmdap_req reqs[CMDAP_MAX]; int n; int default_timeout_ms; } CmdApproval;
void cmdapproval_init(CmdApproval *a, int default_timeout_ms) {
    if(!a) return; memset(a,0,sizeof(*a)); a->default_timeout_ms=default_timeout_ms>0?default_timeout_ms:300000;
}
int cmdapproval_request(CmdApproval *a, const char *command, const char *requester, uint64_t now_ms) {
    if(!a||a->n>=CMDAP_MAX||!command) return -1;
    cmdap_req *r=&a->reqs[a->n]; snprintf(r->command,CMDAP_CMD,"%s",command); snprintf(r->requester,40,"%s",requester?requester:"");
    r->state=CMDAP_PENDING; r->requested_ms=now_ms; r->timeout_ms=a->default_timeout_ms; return a->n++;
}
int cmdapproval_decide(CmdApproval *a, int idx, int approve, const char *approver, uint64_t now_ms) {
    if(!a||idx<0||idx>=a->n||a->reqs[idx].state!=CMDAP_PENDING) return -1;
    if ((now_ms-a->reqs[idx].requested_ms)>(uint64_t)a->reqs[idx].timeout_ms) { a->reqs[idx].state=CMDAP_EXPIRED; return -1; }
    a->reqs[idx].state = approve?CMDAP_APPROVED:CMDAP_DENIED;
    snprintf(a->reqs[idx].approver,40,"%s",approver?approver:""); return 0;
}
int cmdapproval_state(const CmdApproval *a, int idx, uint64_t now_ms) {
    if(!a||idx<0||idx>=a->n) return -1;
    if (a->reqs[idx].state==CMDAP_PENDING && (now_ms-a->reqs[idx].requested_ms)>(uint64_t)a->reqs[idx].timeout_ms) return CMDAP_EXPIRED;
    return a->reqs[idx].state;
}
int cmdapproval_pending_count(const CmdApproval *a, uint64_t now_ms) {
    if(!a) return -1; int n=0;
    for (int i=0;i<a->n;i++) if (cmdapproval_state(a,i,now_ms)==CMDAP_PENDING) n++;
    return n;
}
