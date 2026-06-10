/* puttyalt_import2.c - Config import from PuTTY/OpenSSH/JSON formats. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
enum im_format { IM_UNKNOWN=0, IM_PUTTY=1, IM_SSHCONFIG=2, IM_JSON=3 };
typedef struct { char host[160]; int port; char user[64]; char keyfile[160]; } im_session;
int import2_detect(const char *content) {
    if(!content) return IM_UNKNOWN;
    if (strstr(content,"HostName=")||strstr(content,"PortNumber=")) return IM_PUTTY;
    if (strstr(content,"Host ")||strstr(content,"HostName ")) return IM_SSHCONFIG;
    if (content[0]=='{'||content[0]=='[') return IM_JSON;
    return IM_UNKNOWN;
}
int import2_putty(const char *content, im_session *out) {
    if(!content||!out) return -1; memset(out,0,sizeof(*out)); out->port=22;
    const char *p;
    if ((p=strstr(content,"HostName="))) sscanf(p+9,"%159[^\n\r]",out->host);
    if ((p=strstr(content,"PortNumber="))) out->port=atoi(p+11);
    if ((p=strstr(content,"UserName="))) sscanf(p+9,"%63[^\n\r]",out->user);
    return out->host[0]?0:-1;
}
int import2_sshconfig(const char *content, im_session *out) {
    if(!content||!out) return -1; memset(out,0,sizeof(*out)); out->port=22;
    const char *p;
    if ((p=strstr(content,"HostName "))) sscanf(p+9,"%159s",out->host);
    if ((p=strstr(content,"Port "))) out->port=atoi(p+5);
    if ((p=strstr(content,"User "))) sscanf(p+5,"%63s",out->user);
    if ((p=strstr(content,"IdentityFile "))) sscanf(p+13,"%159s",out->keyfile);
    return out->host[0]?0:-1;
}
