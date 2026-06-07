/* puttyalt_sessscore.c - Session health scoring (latency/errors/uptime composite). */
#include <string.h>
typedef struct { int latency_ms; int error_count; int reconnects; long uptime_sec; long total_sec; int score; } SessScore;
void sessscore_init(SessScore *s) { if(s) memset(s,0,sizeof(*s)); }
void sessscore_update(SessScore *s, int latency_ms, int errors, int reconnects, long uptime, long total) {
    if(!s) return; s->latency_ms=latency_ms; s->error_count=errors; s->reconnects=reconnects;
    s->uptime_sec=uptime; s->total_sec=total;
}
int sessscore_compute(SessScore *s) {
    if(!s) return -1;
    int score=100;
    /* latency penalty */
    if (s->latency_ms>500) score-=30; else if (s->latency_ms>200) score-=15; else if (s->latency_ms>100) score-=5;
    /* error penalty */
    score -= s->error_count*3; if (score<0) score=0;
    /* reconnect penalty */
    score -= s->reconnects*5;
    /* uptime ratio bonus/penalty */
    if (s->total_sec>0) {
        int ratio = (int)((s->uptime_sec*100)/s->total_sec);
        if (ratio<50) score-=20; else if (ratio<80) score-=10;
    }
    if (score<0) score=0; if (score>100) score=100;
    s->score=score; return score;
}
const char *sessscore_grade(const SessScore *s) {
    if(!s) return "?";
    if (s->score>=90) return "A"; if (s->score>=75) return "B";
    if (s->score>=60) return "C"; if (s->score>=40) return "D"; return "F";
}
