/* puttyalt_sessexport2.c - Session export v2 to multiple formats (JSON/YAML/sshconfig). */
#include <string.h>
#include <stdio.h>
enum se_fmt { SE_JSON=0, SE_YAML=1, SE_SSHCONFIG=2, SE_PUTTY=3 };
typedef struct { char name[64]; char host[160]; int port; char user[64]; char keyfile[160]; } se_session;
int sessexport2_one(const se_session *s, int fmt, char *out, int outlen) {
    if(!s||!out) return -1;
    switch(fmt) {
        case SE_JSON:
            return snprintf(out,outlen,"{\"name\":\"%s\",\"host\":\"%s\",\"port\":%d,\"user\":\"%s\"}",s->name,s->host,s->port,s->user);
        case SE_YAML:
            return snprintf(out,outlen,"- name: %s\n  host: %s\n  port: %d\n  user: %s\n",s->name,s->host,s->port,s->user);
        case SE_SSHCONFIG:
            return snprintf(out,outlen,"Host %s\n    HostName %s\n    Port %d\n    User %s\n%s%s%s",
                s->name,s->host,s->port,s->user,
                s->keyfile[0]?"    IdentityFile ":"",s->keyfile,s->keyfile[0]?"\n":"");
        default:
            return snprintf(out,outlen,"[%s]\nHostName=%s\nPortNumber=%d\nUserName=%s\n",s->name,s->host,s->port,s->user);
    }
}
const char *sessexport2_extension(int fmt) {
    switch(fmt){ case SE_JSON:return ".json"; case SE_YAML:return ".yaml"; case SE_SSHCONFIG:return ".sshconfig"; default:return ".reg"; }
}
