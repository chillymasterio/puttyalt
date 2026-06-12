/* puttyalt_jsonbuild.c - JSON builder (object/array/value emission). */
#include <string.h>
#include <stdio.h>
typedef struct { char buf[1024]; int len; int depth; int need_comma; } JsonBuild;
void jsonbuild_init(JsonBuild *j) { if(j) memset(j,0,sizeof(*j)); }
static void jb_comma(JsonBuild *j) { if(j->need_comma && j->len<1023){ j->buf[j->len++]=','; } j->need_comma=0; }
int jsonbuild_obj_start(JsonBuild *j) { if(!j) return -1; jb_comma(j); if(j->len<1023) j->buf[j->len++]='{'; j->depth++; return 0; }
int jsonbuild_obj_end(JsonBuild *j) { if(!j||j->depth==0) return -1; if(j->len<1023) j->buf[j->len++]='}'; j->depth--; j->need_comma=1; return 0; }
int jsonbuild_arr_start(JsonBuild *j) { if(!j) return -1; jb_comma(j); if(j->len<1023) j->buf[j->len++]='['; j->depth++; return 0; }
int jsonbuild_arr_end(JsonBuild *j) { if(!j||j->depth==0) return -1; if(j->len<1023) j->buf[j->len++]=']'; j->depth--; j->need_comma=1; return 0; }
int jsonbuild_key_str(JsonBuild *j, const char *key, const char *val) {
    if(!j||!key) return -1;
    jb_comma(j);
    j->len+=snprintf(j->buf+j->len,1024-j->len,"\"%s\":\"%s\"",key,val?val:"");
    j->need_comma=1; return 0;
}
int jsonbuild_key_int(JsonBuild *j, const char *key, long val) {
    if(!j||!key) return -1;
    jb_comma(j);
    j->len+=snprintf(j->buf+j->len,1024-j->len,"\"%s\":%ld",key,val);
    j->need_comma=1; return 0;
}
const char *jsonbuild_result(const JsonBuild *j) { return j?j->buf:""; }
int jsonbuild_length(const JsonBuild *j) { return j?j->len:-1; }
