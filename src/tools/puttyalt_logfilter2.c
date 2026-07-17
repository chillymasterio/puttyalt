/* puttyalt_logfilter2.c - Filter log lines by level and pattern.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
typedef struct { int min_level; char pattern[48]; int has_pattern; } LogFilter2;
void lgf_init(LogFilter2 *f,int min_level){ if(f){f->min_level=min_level;f->has_pattern=0;f->pattern[0]=0;} }
void lgf_pattern(LogFilter2 *f,const char *pat){ if(!f||!pat)return; strncpy(f->pattern,pat,47); f->pattern[47]=0; f->has_pattern=1; }
int lgf_pass(const LogFilter2 *f,int level,const char *msg){ if(!f)return 1; if(level<f->min_level)return 0; if(f->has_pattern&&msg&&!strstr(msg,f->pattern))return 0; return 1; }
