/* puttyalt_sshtunnel.c - SSH tunnel lifecycle manager with auto-restart. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define ST_MAX 24
#define ST_NAME 64
enum st_state { STT_DOWN=0, STT_CONNECTING=1, STT_UP=2, STT_FAILED=3 };
typedef struct { char name[ST_NAME]; int state; int auto_restart; int restart_count; uint64_t last_change; } st_tunnel;
typedef struct { st_tunnel t[ST_MAX]; int n; } SshTunnel;
void sshtunnel_init(SshTunnel *s) { if(s) memset(s,0,sizeof(*s)); }
int sshtunnel_add(SshTunnel *s, const char *name, int auto_restart) {
    if(!s||s->n>=ST_MAX||!name) return -1;
    st_tunnel *t=&s->t[s->n]; snprintf(t->name,ST_NAME,"%s",name); t->auto_restart=auto_restart?1:0; t->state=STT_DOWN;
    return s->n++;
}
int sshtunnel_set_state(SshTunnel *s, int idx, int state, uint64_t now_ms) {
    if(!s||idx<0||idx>=s->n) return -1;
    s->t[idx].state=state; s->t[idx].last_change=now_ms; return 0;
}
int sshtunnel_should_restart(SshTunnel *s, int idx, uint64_t now_ms) {
    if(!s||idx<0||idx>=s->n) return 0;
    if (s->t[idx].state==STT_FAILED && s->t[idx].auto_restart) {
        if ((now_ms - s->t[idx].last_change) >= 5000) { s->t[idx].restart_count++; return 1; }
    }
    return 0;
}
int sshtunnel_up_count(const SshTunnel *s) {
    if(!s) return -1;
    int n=0; for(int i=0;i<s->n;i++) if(s->t[i].state==STT_UP)n++; return n;
}
int ssttunnel_count(const SshTunnel *s) { return s?s->n:-1; }
