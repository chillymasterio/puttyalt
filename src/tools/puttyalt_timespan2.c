/* puttyalt_timespan2.c - Duration arithmetic and breakdown.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { int days,hours,mins,secs; } TimeSpan;
TimeSpan tsp_break(long seconds){ TimeSpan t; if(seconds<0)seconds=0; t.days=(int)(seconds/86400); t.hours=(int)(seconds%86400/3600); t.mins=(int)(seconds%3600/60); t.secs=(int)(seconds%60); return t; }
long tsp_total(TimeSpan t){ return (long)t.days*86400+t.hours*3600+t.mins*60+t.secs; }
long tsp_add(long a,long b){ return a+b; }
