#include <string.h>
#include <stdio.h>
#include <stdlib.h>
typedef struct { char host[256]; int port; char user[64]; char keyfile[256]; char session[128]; int verbose; int portable; char config[512]; char command[1024]; } CliArgs;
int cli_parse(int argc, char **argv, CliArgs *args) {
    memset(args, 0, sizeof(*args)); args->port = 22;
    for (int i=1;i<argc;i++) {
        if (strcmp(argv[i],"-p")==0 && i+1<argc) args->port = atoi(argv[++i]);
        else if (strcmp(argv[i],"-l")==0 && i+1<argc) snprintf(args->user,64,"%s",argv[++i]);
        else if (strcmp(argv[i],"-i")==0 && i+1<argc) snprintf(args->keyfile,256,"%s",argv[++i]);
        else if (strcmp(argv[i],"-load")==0 && i+1<argc) snprintf(args->session,128,"%s",argv[++i]);
        else if (strcmp(argv[i],"-e")==0 && i+1<argc) snprintf(args->command,1024,"%s",argv[++i]);
        else if (strcmp(argv[i],"-v")==0) args->verbose=1;
        else if (strcmp(argv[i],"-portable")==0) args->portable=1;
        else if (strcmp(argv[i],"-config")==0 && i+1<argc) snprintf(args->config,512,"%s",argv[++i]);
        else if (argv[i][0]!='-') { char *at=strchr(argv[i],'@'); char *colon=strrchr(argv[i],':');
            if (at) { *at=0; snprintf(args->user,64,"%s",argv[i]); snprintf(args->host,256,"%s",at+1); }
            else snprintf(args->host,256,"%s",argv[i]);
            if (colon && colon>args->host) { *colon=0; int p=atoi(colon+1); if (p>0) args->port=p; }
        }
    }
    return args->host[0] ? 0 : -1;
}
int cli_format_usage(char *buf, int buflen) {
    return snprintf(buf, buflen, "Usage: puttyalt [options] [user@]host[:port]\n\n  -p port\n  -l user\n  -i keyfile\n  -load session\n  -e command\n  -v verbose\n  -portable\n  -config file\n");
}
