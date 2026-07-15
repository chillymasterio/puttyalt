/* puttyalt_permrole.c - Role-based permission checks.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define PR_MAX 8
typedef struct { char roles[PR_MAX][24]; int n; } RoleSet;
void prl_init(RoleSet *r){ if(r)r->n=0; }
int prl_grant(RoleSet *r,const char *role){ if(!r||r->n>=PR_MAX)return -1; strncpy(r->roles[r->n],role,23); r->roles[r->n][23]=0; return r->n++; }
int prl_has(const RoleSet *r,const char *role){ if(!r)return 0; for(int i=0;i<r->n;i++)if(!strcmp(r->roles[i],role))return 1; return 0; }
int prl_has_any(const RoleSet *r,const char *const *roles,int n){ for(int i=0;i<n;i++)if(prl_has(r,roles[i]))return 1; return 0; }
