/* puttyalt_kvparse2.c - Parse key=value with comment/section support.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#include <ctype.h>
enum { KVP_KV, KVP_SECTION, KVP_COMMENT, KVP_BLANK };
typedef struct { int kind; char key[48]; char value[128]; } KvpLine;
int kvp_parse(const char *line, KvpLine *out) {
    if (!line || !out) return -1;
    while (isspace((unsigned char)*line)) line++;
    if (*line==0) { out->kind=KVP_BLANK; return 0; }
    if (*line=='#'||*line==';') { out->kind=KVP_COMMENT; return 0; }
    if (*line=='[') { out->kind=KVP_SECTION; int o=0; line++; while(*line&&*line!=']'&&o<47)out->key[o++]=*line++; out->key[o]=0; return 0; }
    const char *eq=strchr(line,'=');
    if (!eq) return -1;
    out->kind=KVP_KV;
    int kl=(int)(eq-line); while(kl>0&&isspace((unsigned char)line[kl-1]))kl--; if(kl>47)kl=47;
    memcpy(out->key,line,kl); out->key[kl]=0;
    const char *v=eq+1; while(isspace((unsigned char)*v))v++;
    strncpy(out->value,v,127); out->value[127]=0;
    int vl=(int)strlen(out->value); while(vl>0&&isspace((unsigned char)out->value[vl-1]))out->value[--vl]=0;
    return 0;
}
