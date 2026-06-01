/* puttyalt_clipfmt.c - Clipboard content format detection (URL/path/json/code). */
#include <string.h>
#include <stdio.h>
enum clipfmt { CF_TEXT=0, CF_URL, CF_PATH, CF_JSON, CF_CODE, CF_NUMBER };
int clipfmt_detect(const char *text) {
    if(!text||!*text) return CF_TEXT;
    if (!strncmp(text,"http://",7)||!strncmp(text,"https://",8)||!strncmp(text,"ssh://",6)) return CF_URL;
    if (text[0]=='/'||text[0]=='~'||(text[1]==':'&&(text[2]=='\\'||text[2]=='/'))) return CF_PATH;
    if ((text[0]=='{'||text[0]=='[')) return CF_JSON;
    int digits=1; for (const char *p=text;*p;p++) if (*p<'0'||*p>'9') { digits=0; break; }
    if (digits) return CF_NUMBER;
    if (strstr(text,"function")||strstr(text,"def ")||strstr(text,"{")||strstr(text,";")) return CF_CODE;
    return CF_TEXT;
}
int clipfmt_name(int fmt, char *buf, int buflen) {
    static const char *n[]={"text","url","path","json","code","number"};
    if (fmt<0||fmt>CF_NUMBER) return -1;
    return snprintf(buf,buflen,"%s",n[fmt]);
}
