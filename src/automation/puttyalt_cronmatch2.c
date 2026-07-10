/* puttyalt_cronmatch2.c - Match time against cron minute/hour fields.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
static int crm_field(int val,int cron){ return cron<0||cron==val; }
int crm_matches(int cron_min,int cron_hour,int cron_dow,int min,int hour,int dow){
    return crm_field(min,cron_min)&&crm_field(hour,cron_hour)&&crm_field(dow,cron_dow);
}
