/* puttyalt_aiagent.c - Autonomous AI agent loop state (plan/act/observe). */
#include <string.h>
#include <stdio.h>
#define AG_MAX_STEPS 24
#define AG_TEXT 256
enum ag_phase { AG_PLAN=0, AG_ACT=1, AG_OBSERVE=2, AG_DONE=3, AG_HALT=4 };
typedef struct { char action[AG_TEXT]; char result[AG_TEXT]; int success; } ag_step;
typedef struct { ag_step steps[AG_MAX_STEPS]; int n; int phase; int max_steps; char goal[AG_TEXT]; } AiAgent;
void aiagent_init(AiAgent *a, const char *goal, int max_steps) {
    if(!a) return;
    memset(a,0,sizeof(*a)); snprintf(a->goal,AG_TEXT,"%s",goal?goal:"");
    a->max_steps=max_steps>0?max_steps:10; a->phase=AG_PLAN;
}
int aiagent_plan_action(AiAgent *a, const char *action) {
    if(!a||a->phase!=AG_PLAN||a->n>=a->max_steps||a->n>=AG_MAX_STEPS) return -1;
    snprintf(a->steps[a->n].action,AG_TEXT,"%s",action?action:""); a->phase=AG_ACT; return a->n;
}
int aiagent_observe(AiAgent *a, const char *result, int success) {
    if(!a||a->phase!=AG_ACT) return -1;
    snprintf(a->steps[a->n].result,AG_TEXT,"%s",result?result:""); a->steps[a->n].success=success?1:0;
    a->n++; a->phase = (a->n>=a->max_steps)?AG_HALT:AG_PLAN; return 0;
}
void aiagent_complete(AiAgent *a) { if(a) a->phase=AG_DONE; }
int aiagent_phase(const AiAgent *a) { return a?a->phase:-1; }
int aiagent_step_count(const AiAgent *a) { return a?a->n:-1; }
int aiagent_success_rate(const AiAgent *a) {
    if(!a||a->n==0) return 0;
    int ok=0; for(int i=0;i<a->n;i++) if(a->steps[i].success)ok++; return (ok*100)/a->n;
}
