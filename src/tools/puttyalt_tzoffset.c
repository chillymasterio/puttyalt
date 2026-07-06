/* puttyalt_tzoffset.c - Timezone offset formatting.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdio.h>
int tzo_format(int offset_minutes,char *out,int outlen){ char sign=offset_minutes<0?'-':'+'; int a=offset_minutes<0?-offset_minutes:offset_minutes; return snprintf(out,outlen,"%c%02d:%02d",sign,a/60,a%60); }
int tzo_parse(const char *s,int *out){ if(!s||!out)return -1; int sign=1; if(*s=='-'){sign=-1;s++;} else if(*s=='+')s++; int h,m; if(sscanf(s,"%d:%d",&h,&m)!=2)return -1; *out=sign*(h*60+m); return 0; }
int tzo_to_utc(int local_min,int offset_min){ return local_min-offset_min; }
