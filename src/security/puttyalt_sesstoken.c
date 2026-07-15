/* puttyalt_sesstoken.c - Session token lifecycle.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
typedef struct { uint64_t token; long issued; long ttl; int revoked; } SessToken;
void stk2_issue(SessToken *t,uint64_t token,long now,long ttl){ if(t){t->token=token;t->issued=now;t->ttl=ttl;t->revoked=0;} }
int stk2_valid(const SessToken *t,long now){ return t&&!t->revoked&&(now-t->issued<t->ttl); }
void stk2_revoke(SessToken *t){ if(t)t->revoked=1; }
