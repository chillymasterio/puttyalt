/* puttyalt_typedget.c - Typed getters over string config values.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdlib.h>
#include <string.h>
long tget_int(const char *val,long dflt){ if(!val||!*val)return dflt; char *e; long v=strtol(val,&e,10); return *e==0?v:dflt; }
double tget_float(const char *val,double dflt){ if(!val||!*val)return dflt; char *e; double v=strtod(val,&e); return *e==0?v:dflt; }
int tget_bool(const char *val,int dflt){ if(!val)return dflt; if(!strcasecmp(val,"true")||!strcmp(val,"1")||!strcasecmp(val,"yes"))return 1; if(!strcasecmp(val,"false")||!strcmp(val,"0")||!strcasecmp(val,"no"))return 0; return dflt; }
