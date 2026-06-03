/* puttyalt_distsess.c - Distributed session replication state (leader/follower sync). */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define DS_MAX_FOLLOWERS 16
#define DS_NAME 48
enum ds_role { DS_STANDALONE=0, DS_LEADER=1, DS_FOLLOWER=2 };
typedef struct { char id[DS_NAME]; uint64_t acked_seq; int alive; } ds_follower;
typedef struct { int role; uint64_t commit_seq; ds_follower f[DS_MAX_FOLLOWERS]; int nf; char leader_id[DS_NAME]; } DistSess;
void distsess_init(DistSess *d) { if(d) memset(d,0,sizeof(*d)); }
int distsess_become_leader(DistSess *d, const char *self_id) {
    if(!d||!self_id) return -1; d->role=DS_LEADER; snprintf(d->leader_id,DS_NAME,"%s",self_id); return 0;
}
int distsess_add_follower(DistSess *d, const char *id) {
    if(!d||d->role!=DS_LEADER||d->nf>=DS_MAX_FOLLOWERS||!id) return -1;
    snprintf(d->f[d->nf].id,DS_NAME,"%s",id); d->f[d->nf].alive=1; return d->nf++;
}
uint64_t distsess_propose(DistSess *d) {
    if(!d||d->role!=DS_LEADER) return 0; return ++d->commit_seq;
}
int distsess_ack(DistSess *d, const char *follower_id, uint64_t seq) {
    if(!d||!follower_id) return -1;
    for (int i=0;i<d->nf;i++) if (strcmp(d->f[i].id,follower_id)==0) { d->f[i].acked_seq=seq; return 0; }
    return -1;
}
int distsess_is_committed(const DistSess *d, uint64_t seq) {
    if(!d||d->role!=DS_LEADER) return 0;
    int acked=1; /* leader counts */
    for (int i=0;i<d->nf;i++) if (d->f[i].alive && d->f[i].acked_seq>=seq) acked++;
    int quorum = (d->nf+1)/2 + 1;
    return acked>=quorum ? 1 : 0;
}
int distsess_role(const DistSess *d) { return d?d->role:-1; }
