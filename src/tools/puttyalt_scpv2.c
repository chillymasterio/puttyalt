/* puttyalt_scpv2.c - SCP protocol v2 message parser (C/D/E/T directives). */
#include <string.h>
#include <stdio.h>
enum scp_msg { SCP_FILE=0, SCP_DIR=1, SCP_ENDDIR=2, SCP_TIME=3, SCP_UNKNOWN=4 };
typedef struct { int type; int mode; long size; char name[160]; long mtime, atime; } ScpMsg;
int scpv2_parse(const char *line, ScpMsg *out) {
    if(!line||!out) return -1; memset(out,0,sizeof(*out));
    switch(line[0]) {
        case 'C': out->type=SCP_FILE; return sscanf(line+1,"%o %ld %159s",(unsigned int*)&out->mode,&out->size,out->name)==3?0:-1;
        case 'D': out->type=SCP_DIR; return sscanf(line+1,"%o %ld %159s",(unsigned int*)&out->mode,&out->size,out->name)==3?0:-1;
        case 'E': out->type=SCP_ENDDIR; return 0;
        case 'T': out->type=SCP_TIME; return sscanf(line+1,"%ld %*d %ld",&out->mtime,&out->atime)>=1?0:-1;
        default: out->type=SCP_UNKNOWN; return -1;
    }
}
int scpv2_format_file(int mode, long size, const char *name, char *out, int outlen) {
    return snprintf(out,outlen,"C%04o %ld %s\n",mode&07777,size,name?name:"");
}
const char *scpv2_type_name(int type) {
    static const char *n[]={"file","dir","enddir","time","unknown"};
    return (type>=0&&type<=SCP_UNKNOWN)?n[type]:"?";
}
