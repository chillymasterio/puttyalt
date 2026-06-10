/* puttyalt_connstate.c - Connection state machine (full SSH connect lifecycle). */
#include <string.h>
#include <stdint.h>
#include <stdio.h>
enum cs_state { CS_IDLE=0, CS_RESOLVING=1, CS_CONNECTING=2, CS_HANDSHAKE=3, CS_AUTH=4, CS_CHANNEL=5, CS_READY=6, CS_CLOSING=7, CS_CLOSED=8, CS_ERROR=9 };
typedef struct { int state; int prev_state; uint64_t state_entered_ms; int retry_count; char error[96]; } ConnState;
void connstate_init(ConnState *c) { if(c){ memset(c,0,sizeof(*c)); c->state=CS_IDLE; } }
int connstate_transition(ConnState *c, int new_state, uint64_t now_ms) {
    if(!c) return -1;
    /* validate transition */
    static const int valid_next[10][10]={
        {0,1,0,0,0,0,0,0,1,0}, /* IDLE -> RESOLVING/CLOSED */
        {0,0,1,0,0,0,0,0,0,1}, /* RESOLVING -> CONNECTING/ERROR */
        {0,0,0,1,0,0,0,1,0,1}, /* CONNECTING -> HANDSHAKE/CLOSING/ERROR */
        {0,0,0,0,1,0,0,1,0,1}, /* HANDSHAKE -> AUTH */
        {0,0,0,0,0,1,0,1,0,1}, /* AUTH -> CHANNEL */
        {0,0,0,0,0,0,1,1,0,1}, /* CHANNEL -> READY */
        {0,0,0,0,0,0,0,1,0,1}, /* READY -> CLOSING/ERROR */
        {0,0,0,0,0,0,0,0,1,0}, /* CLOSING -> CLOSED */
        {0,1,0,0,0,0,0,0,0,0}, /* CLOSED -> RESOLVING (reconnect) */
        {0,1,0,0,0,0,0,0,1,0}, /* ERROR -> RESOLVING/CLOSED */
    };
    if (new_state<0||new_state>9||c->state>9) return -1;
    if (!valid_next[c->state][new_state]) return -1;
    c->prev_state=c->state; c->state=new_state; c->state_entered_ms=now_ms;
    if (new_state==CS_RESOLVING && c->prev_state==CS_ERROR) c->retry_count++;
    return 0;
}
void connstate_set_error(ConnState *c, const char *err, uint64_t now_ms) {
    if(!c) return;
    snprintf(c->error,96,"%s",err?err:""); c->prev_state=c->state; c->state=CS_ERROR; c->state_entered_ms=now_ms;
}
int connstate_is_active(const ConnState *c) { return (c&&c->state>=CS_RESOLVING&&c->state<=CS_READY)?1:0; }
int connstate_state(const ConnState *c) { return c?c->state:-1; }
const char *connstate_name(int state) {
    static const char *n[]={"idle","resolving","connecting","handshake","auth","channel","ready","closing","closed","error"};
    return (state>=0&&state<=9)?n[state]:"?";
}
