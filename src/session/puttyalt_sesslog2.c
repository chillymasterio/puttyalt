/* puttyalt_sesslog2.c - Per-session activity log summary.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { long commands; long bytes_sent; long errors; long start; } SessLog2;
void slg_init(SessLog2 *s,long now){ if(s){s->commands=0;s->bytes_sent=0;s->errors=0;s->start=now;} }
void slg_command(SessLog2 *s,long bytes){ if(s){s->commands++;s->bytes_sent+=bytes;} }
long slg_duration(const SessLog2 *s,long now){ return s?now-s->start:0; }
