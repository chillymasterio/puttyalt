/* puttyalt_errctx.c - Error context stack for diagnostics.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define EC_DEPTH 8
typedef struct { char frames[EC_DEPTH][64]; int depth; int code; } ErrCtx;
void erc_init(ErrCtx *e){ if(e){e->depth=0;e->code=0;} }
void erc_push(ErrCtx *e,const char *ctx){ if(!e||e->depth>=EC_DEPTH)return; strncpy(e->frames[e->depth],ctx,63); e->frames[e->depth][63]=0; e->depth++; }
void erc_pop(ErrCtx *e){ if(e&&e->depth>0)e->depth--; }
int erc_format(const ErrCtx *e,char *out,int outlen){ if(!e||!out)return -1; int o=0; for(int i=e->depth-1;i>=0;i--){ int n=(int)strlen(e->frames[i]); for(int k=0;k<n&&o<outlen-3;k++)out[o++]=e->frames[i][k]; if(i>0&&o<outlen-2){out[o++]=' ';out[o++]='>';out[o++]=' ';} } out[o]=0; return o; }
