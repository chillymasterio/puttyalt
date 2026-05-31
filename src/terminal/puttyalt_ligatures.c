#include <string.h>
#include <stdio.h>
#define MAX_LIGATURES 64
typedef struct { char seq[8]; int glyph_id; int enabled; } Ligature;
static Ligature g_lig[MAX_LIGATURES]; static int g_lig_count = 0;
void ligatures_init(void) {
    g_lig_count = 0;
    const char *defaults[][2] = {{"=>","\xe2\x87\x92"},{"->","\xe2\x86\x92"},{"!=","\xe2\x89\xa0"},{"<=","\xe2\x89\xa4"},{">=","\xe2\x89\xa5"},{"==","\xe2\x89\xa1"},{"&&","\xe2\x88\xa7"},{"||","\xe2\x88\xa8"},{NULL,NULL}};
    for (int i=0;defaults[i][0];i++) { snprintf(g_lig[g_lig_count].seq,8,"%s",defaults[i][0]); g_lig[g_lig_count].enabled=1; g_lig_count++; }
}
int ligatures_check(const char *text, int pos, int *skip) {
    for (int i=0;i<g_lig_count;i++) if (g_lig[i].enabled && strncmp(text+pos,g_lig[i].seq,strlen(g_lig[i].seq))==0) { *skip=(int)strlen(g_lig[i].seq); return i; }
    *skip=1; return -1;
}
void ligatures_toggle(int idx, int on) { if (idx>=0&&idx<g_lig_count) g_lig[idx].enabled=on; }
int ligatures_count(void) { return g_lig_count; }
