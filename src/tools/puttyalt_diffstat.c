/* puttyalt_diffstat.c - Diff statistics (additions/deletions/files changed). */
#include <string.h>
#include <stdio.h>
typedef struct { int files; int additions; int deletions; int hunks; } DiffStat;
void diffstat_init(DiffStat *d) { if(d) memset(d,0,sizeof(*d)); }
int diffstat_process_line(DiffStat *d, const char *line) {
    if(!d||!line) return -1;
    if (!strncmp(line,"+++",3)||!strncmp(line,"---",3)) return 0;
    /* file headers */
    if (!strncmp(line,"diff ",5)) { d->files++; return 0; }
    if (!strncmp(line,"@@",2)) { d->hunks++; return 0; }
    if (line[0]=='+') { d->additions++; return 1; }
    if (line[0]=='-') { d->deletions++; return 2; }
    return 0;
}
int diffstat_format(const DiffStat *d, char *buf, int buflen) {
    if(!d||!buf) return -1;
    return snprintf(buf,buflen,"%d file%s changed, %d insertion%s(+), %d deletion%s(-)",
        d->files,d->files==1?"":"s",d->additions,d->additions==1?"":"s",d->deletions,d->deletions==1?"":"s");
}
int diffstat_net(const DiffStat *d) { return d? (d->additions - d->deletions):0; }
