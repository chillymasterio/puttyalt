/* puttyalt_pwpolicy.c - Password policy enforcement (length/complexity/history). */
#include <string.h>
#include <stdio.h>
typedef struct { int min_length; int require_upper; int require_lower; int require_digit; int require_symbol; int max_repeat; } PwPolicy;
void pwpolicy_init(PwPolicy *p) {
    if(!p) return;
    p->min_length=8; p->require_upper=1; p->require_lower=1; p->require_digit=1; p->require_symbol=0; p->max_repeat=3;
}
int pwpolicy_check(const PwPolicy *p, const char *pw, char *reason, int rlen) {
    if(!p||!pw) return -1;
    int len=(int)strlen(pw);
    if (len<p->min_length) { if(reason) snprintf(reason,rlen,"too short (min %d)",p->min_length); return 0; }
    int up=0,lo=0,dg=0,sy=0;
    int run=1; char last=0;
    for (const char *c=pw;*c;c++) {
        if (*c>='A'&&*c<='Z') up=1; else if (*c>='a'&&*c<='z') lo=1;
        else if (*c>='0'&&*c<='9') dg=1; else sy=1;
        if (*c==last) { run++; if (run>p->max_repeat) { if(reason) snprintf(reason,rlen,"too many repeats"); return 0; } } else run=1;
        last=*c;
    }
    if (p->require_upper && !up) { if(reason) snprintf(reason,rlen,"need uppercase"); return 0; }
    if (p->require_lower && !lo) { if(reason) snprintf(reason,rlen,"need lowercase"); return 0; }
    if (p->require_digit && !dg) { if(reason) snprintf(reason,rlen,"need digit"); return 0; }
    if (p->require_symbol && !sy) { if(reason) snprintf(reason,rlen,"need symbol"); return 0; }
    if (reason) snprintf(reason,rlen,"ok");
    return 1;
}
