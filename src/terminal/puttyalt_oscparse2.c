/* puttyalt_oscparse2.c - OSC sequence parser v2 (numeric + string params). */
#include <string.h>
#include <stdlib.h>
#define OS_PARAM 512
typedef struct { int command; char param[OS_PARAM]; int terminated; } OscParse2;
void oscparse2_init(OscParse2 *o) { if(o) memset(o,0,sizeof(*o)); }
int oscparse2_parse(OscParse2 *o, const char *data, int len) {
    if(!o||!data) return -1; memset(o,0,sizeof(*o));
    /* data is content between ESC] and ST/BEL */
    int i=0; o->command=0;
    while (i<len && data[i]>='0' && data[i]<='9') { o->command=o->command*10+(data[i]-'0'); i++; }
    if (i<len && data[i]==';') i++;
    int p=0;
    while (i<len && p<OS_PARAM-1 && data[i]!='\x07' && data[i]!='\x1b') o->param[p++]=data[i++];
    o->param[p]=0;
    o->terminated = (i<len && (data[i]=='\x07'||data[i]=='\x1b'));
    return 0;
}
int oscparse2_command(const OscParse2 *o) { return o?o->command:-1; }
const char *oscparse2_param(const OscParse2 *o) { return o?o->param:""; }
/* common OSC commands: 0/2=title, 4=palette, 8=hyperlink, 52=clipboard, 10/11=colors */
int oscparse2_classify(const OscParse2 *o) {
    if(!o) return -1;
    switch(o->command) { case 0: case 2: return 1; /* title */ case 4: return 2; /* palette */
        case 8: return 3; /* link */ case 52: return 4; /* clip */ case 10: case 11: case 12: return 5; default: return 0; }
}
