/* puttyalt_cronexpr.c - Cron expression matcher (minute hour dom month dow). */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
typedef struct { char minute[16]; char hour[16]; char dom[16]; char month[16]; char dow[16]; } CronExpr;
int cronexpr_parse(const char *expr, CronExpr *out) {
    if(!expr||!out) return -1;
    memset(out,0,sizeof(*out));
    return sscanf(expr,"%15s %15s %15s %15s %15s",out->minute,out->hour,out->dom,out->month,out->dow)==5 ? 0 : -1;
}
static int cron_field_match(const char *field, int value) {
    if (strcmp(field,"*")==0) return 1;
    /* step syntax: asterisk slash N */
    if (field[0]=='*' && field[1]=='/') { int step=atoi(field+2); return step>0 && value%step==0; }
    /* a-b range */
    const char *dash=strchr(field,'-');
    if (dash) { int lo=atoi(field), hi=atoi(dash+1); return value>=lo && value<=hi; }
    /* comma list */
    const char *p=field;
    while (*p) { int v=atoi(p); if (v==value) return 1; const char *c=strchr(p,','); if(!c) break; p=c+1; }
    return atoi(field)==value;
}
int cronexpr_matches(const CronExpr *c, int minute, int hour, int dom, int month, int dow) {
    if(!c) return 0;
    return cron_field_match(c->minute,minute) && cron_field_match(c->hour,hour) &&
           cron_field_match(c->dom,dom) && cron_field_match(c->month,month) && cron_field_match(c->dow,dow);
}
