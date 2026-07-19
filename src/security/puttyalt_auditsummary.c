/* puttyalt_auditsummary.c - Summarize audit events by category.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define AS_MAX 16
typedef struct { char category[24]; long count; } AsCat;
typedef struct { AsCat cats[AS_MAX]; int n; long total; } AuditSummary;
void aus_init(AuditSummary *a){ if(a){a->n=0;a->total=0;} }
void aus_record(AuditSummary *a,const char *category){ if(!a)return; a->total++; for(int i=0;i<a->n;i++)if(!strcmp(a->cats[i].category,category)){a->cats[i].count++;return;} if(a->n<AS_MAX){strncpy(a->cats[a->n].category,category,23);a->cats[a->n].category[23]=0;a->cats[a->n].count=1;a->n++;} }
long aus_count(const AuditSummary *a,const char *category){ if(!a)return 0; for(int i=0;i<a->n;i++)if(!strcmp(a->cats[i].category,category))return a->cats[i].count; return 0; }
