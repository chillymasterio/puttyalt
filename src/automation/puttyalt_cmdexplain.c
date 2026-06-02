/* puttyalt_cmdexplain.c - Heuristic command explainer (flags/pipes/danger tags). */
#include <string.h>
#include <stdio.h>
enum ce_flag { CE_SAFE=0, CE_PIPE=1, CE_REDIR=2, CE_SUDO=4, CE_DANGER=8, CE_NETWORK=16 };
int cmdexplain_classify(const char *cmd) {
    if(!cmd) return CE_SAFE; int f=CE_SAFE;
    if (strstr(cmd,"|")) f|=CE_PIPE;
    if (strstr(cmd,">")||strstr(cmd,">>")) f|=CE_REDIR;
    if (!strncmp(cmd,"sudo ",5)||strstr(cmd," sudo ")) f|=CE_SUDO;
    if (strstr(cmd,"rm -rf")||strstr(cmd,"mkfs")||strstr(cmd,"dd if=")||strstr(cmd,":(){")||strstr(cmd,"> /dev/sd")) f|=CE_DANGER;
    if (strstr(cmd,"curl")||strstr(cmd,"wget")||strstr(cmd,"ssh ")||strstr(cmd,"scp ")||strstr(cmd,"nc ")) f|=CE_NETWORK;
    return f;
}
int cmdexplain_describe(const char *cmd, char *buf, int buflen) {
    if(!cmd||!buf) return -1; int f=cmdexplain_classify(cmd); int pos=0;
    if (f&CE_DANGER) pos+=snprintf(buf+pos,buflen-pos,"[DANGER] ");
    if (f&CE_SUDO) pos+=snprintf(buf+pos,buflen-pos,"elevated ");
    if (f&CE_NETWORK) pos+=snprintf(buf+pos,buflen-pos,"network ");
    if (f&CE_PIPE) pos+=snprintf(buf+pos,buflen-pos,"piped ");
    if (f&CE_REDIR) pos+=snprintf(buf+pos,buflen-pos,"redirect ");
    if (pos==0) pos+=snprintf(buf+pos,buflen-pos,"safe");
    return pos;
}
int cmdexplain_is_dangerous(const char *cmd) { return (cmdexplain_classify(cmd)&CE_DANGER)?1:0; }
