/* puttyalt_cmdsafety.c - Command safety analyzer with risk scoring + suggestions. */
#include <string.h>
#include <stdio.h>
typedef struct { int risk_score; int destructive; int needs_sudo; int affects_system; char reason[128]; } CmdSafety;
int cmdsafety_analyze(const char *cmd, CmdSafety *out) {
    if(!cmd||!out) return -1; memset(out,0,sizeof(*out));
    int score=0;
    if (strstr(cmd,"rm -rf /")||strstr(cmd,"rm -rf /*")) { score+=90; out->destructive=1; snprintf(out->reason,128,"recursive delete from root"); }
    else if (strstr(cmd,"rm -rf")) { score+=40; out->destructive=1; snprintf(out->reason,128,"recursive force delete"); }
    if (strstr(cmd,"mkfs")||strstr(cmd,"dd if=")||strstr(cmd,"> /dev/sd")) { score+=80; out->destructive=1; snprintf(out->reason,128,"disk-level operation"); }
    if (strstr(cmd,":(){")||strstr(cmd,"fork")) { score+=70; snprintf(out->reason,128,"possible fork bomb"); }
    if (!strncmp(cmd,"sudo ",5)) { out->needs_sudo=1; score+=10; }
    if (strstr(cmd,"chmod 777")||strstr(cmd,"chmod -R 777")) { score+=30; out->affects_system=1; if(!out->reason[0]) snprintf(out->reason,128,"world-writable permissions"); }
    if (strstr(cmd,"/etc/")||strstr(cmd,"systemctl")||strstr(cmd,"service ")) { out->affects_system=1; score+=15; }
    if (out->reason[0]==0) snprintf(out->reason,128,"no obvious risk");
    out->risk_score = score>100?100:score;
    return 0;
}
int cmdsafety_should_confirm(const CmdSafety *s) { return (s&&s->risk_score>=40)?1:0; }
int cmdsafety_is_blocked(const CmdSafety *s) { return (s&&s->risk_score>=85)?1:0; }
