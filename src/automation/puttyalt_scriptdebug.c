/* puttyalt_scriptdebug.c - Script debugger: breakpoints + step + watch. */
#include <string.h>
#include <stdio.h>
#define SD_BREAKS 32
#define SD_WATCH 16
#define SD_NAME 32
enum sd_state { SD_RUNNING=0, SD_PAUSED=1, SD_STEPPING=2, SD_STOPPED=3 };
typedef struct { int line; int enabled; int hit_count; } sd_break;
typedef struct { char name[SD_NAME]; char value[64]; } sd_watch;
typedef struct { sd_break breaks[SD_BREAKS]; int nbreaks; sd_watch watches[SD_WATCH]; int nwatches; int state; int current_line; } ScriptDebug;
void scriptdebug_init(ScriptDebug *s) { if(s){ memset(s,0,sizeof(*s)); s->state=SD_STOPPED; } }
int scriptdebug_add_break(ScriptDebug *s, int line) {
    if(!s||s->nbreaks>=SD_BREAKS) return -1;
    s->breaks[s->nbreaks].line=line; s->breaks[s->nbreaks].enabled=1; return s->nbreaks++;
}
int scriptdebug_should_break(ScriptDebug *s, int line) {
    if(!s) return 0;
    if (s->state==SD_STEPPING) return 1;
    for (int i=0;i<s->nbreaks;i++) if (s->breaks[i].enabled && s->breaks[i].line==line) {
        s->breaks[i].hit_count++; s->state=SD_PAUSED; s->current_line=line; return 1;
    }
    return 0;
}
void scriptdebug_continue(ScriptDebug *s) { if(s) s->state=SD_RUNNING; }
void scriptdebug_step(ScriptDebug *s) { if(s) s->state=SD_STEPPING; }
int scriptdebug_set_watch(ScriptDebug *s, const char *name, const char *value) {
    if(!s||!name) return -1;
    for (int i=0;i<s->nwatches;i++) if (strcmp(s->watches[i].name,name)==0) { snprintf(s->watches[i].value,64,"%s",value?value:""); return 0; }
    if (s->nwatches>=SD_WATCH) return -1;
    snprintf(s->watches[s->nwatches].name,SD_NAME,"%s",name); snprintf(s->watches[s->nwatches].value,64,"%s",value?value:""); s->nwatches++;
    return 0;
}
int scriptdebug_state(const ScriptDebug *s) { return s?s->state:-1; }
