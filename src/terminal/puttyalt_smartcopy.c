/* puttyalt_smartcopy.c - Smart copy: auto-detect & extract URLs/paths/IPs from selection. */
#include <string.h>
#include <stdio.h>
enum sc_type { SC_PLAIN=0, SC_URL=1, SC_PATH=2, SC_IP=3, SC_EMAIL=4, SC_HASH=5 };
int smartcopy_detect(const char *text) {
    if(!text) return SC_PLAIN;
    if (!strncmp(text,"http://",7)||!strncmp(text,"https://",8)) return SC_URL;
    if (strchr(text,'@') && strchr(text,'.')) return SC_EMAIL;
    if (text[0]=='/'||(text[1]==':'&&text[2]=='\\')) return SC_PATH;
    /* IP: digits and dots, 3 dots */
    int dots=0, digits=0, other=0;
    for (const char *p=text;*p;p++) { if(*p=='.')dots++; else if(*p>='0'&&*p<='9')digits++; else other++; }
    if (dots==3 && digits>=4 && other==0) return SC_IP;
    /* hash: all hex, length 32/40/64 */
    int len=(int)strlen(text), hex=1;
    for (const char *p=text;*p;p++) if(!((*p>='0'&&*p<='9')||(*p>='a'&&*p<='f')||(*p>='A'&&*p<='F'))) { hex=0; break; }
    if (hex && (len==32||len==40||len==64)) return SC_HASH;
    return SC_PLAIN;
}
int smartcopy_action_hint(int type, char *buf, int buflen) {
    static const char *n[]={"copy","open URL","open path","ping/connect","compose email","verify hash"};
    if (type<0||type>SC_HASH) return -1;
    return snprintf(buf,buflen,"%s",n[type]);
}
int smartcopy_trim(const char *text, char *out, int outlen) {
    if(!text||!out) return -1;
    const char *start=text; while (*start==' '||*start=='\t'||*start=='\n') start++;
    int len=(int)strlen(start);
    while (len>0 && (start[len-1]==' '||start[len-1]=='\n'||start[len-1]=='\t'||start[len-1]=='\r')) len--;
    if (len>=outlen) len=outlen-1;
    memcpy(out,start,len); out[len]=0; return len;
}
