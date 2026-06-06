/* puttyalt_secretscan.c - Outbound secret/credential leak scanner for paste/input. */
#include <string.h>
#include <stdio.h>
enum ss_kind { SS_NONE=0, SS_AWS_KEY=1, SS_PRIVATE_KEY=2, SS_TOKEN=3, SS_PASSWORD=4, SS_CARD=5 };
int secretscan_classify(const char *text) {
    if(!text) return SS_NONE;
    if (strstr(text,"AKIA")||strstr(text,"aws_secret")) return SS_AWS_KEY;
    if (strstr(text,"BEGIN RSA PRIVATE")||strstr(text,"BEGIN OPENSSH PRIVATE")||strstr(text,"BEGIN PRIVATE")) return SS_PRIVATE_KEY;
    if (strstr(text,"ghp_")||strstr(text,"xoxb-")||strstr(text,"Bearer ")) return SS_TOKEN;
    if (strstr(text,"password=")||strstr(text,"passwd=")) return SS_PASSWORD;
    /* naive card: 16 consecutive digits */
    int run=0; for (const char *p=text;*p;p++) { if(*p>='0'&&*p<='9'){if(++run>=16)return SS_CARD;} else run=0; }
    return SS_NONE;
}
int secretscan_is_risky(const char *text) { return secretscan_classify(text)!=SS_NONE?1:0; }
int secretscan_describe(int kind, char *buf, int buflen) {
    static const char *n[]={"none","AWS key","private key","API token","password","card number"};
    if (kind<0||kind>SS_CARD) return -1;
    return snprintf(buf,buflen,"%s",n[kind]);
}
int secretscan_redact(const char *text, char *out, int outlen) {
    if(!text||!out) return -1;
    int kind=secretscan_classify(text);
    if (kind==SS_NONE) return snprintf(out,outlen,"%s",text);
    return snprintf(out,outlen,"[REDACTED: %s]", kind==SS_AWS_KEY?"AWS key":kind==SS_PRIVATE_KEY?"private key":kind==SS_TOKEN?"token":kind==SS_PASSWORD?"password":"card");
}
