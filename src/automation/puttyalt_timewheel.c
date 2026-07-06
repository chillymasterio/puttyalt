/* puttyalt_timewheel.c - Hashed timer wheel for timeouts.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
#define TW_SLOTS 16
typedef struct { long deadline; int id; int active; } TwTimer;
typedef struct { TwTimer slots[TW_SLOTS][8]; long tick_ms; } TimeWheel;
void twl_init(TimeWheel *w,long tick_ms){ if(!w)return; w->tick_ms=tick_ms>0?tick_ms:100; for(int i=0;i<TW_SLOTS;i++)for(int j=0;j<8;j++)w->slots[i][j].active=0; }
int twl_add(TimeWheel *w,int id,long deadline){ if(!w)return -1; int slot=(int)((deadline/w->tick_ms)%TW_SLOTS); for(int j=0;j<8;j++)if(!w->slots[slot][j].active){w->slots[slot][j].id=id;w->slots[slot][j].deadline=deadline;w->slots[slot][j].active=1;return 0;} return -1; }
int twl_expired(TimeWheel *w,long now,int *out,int cap){ if(!w)return 0; int c=0; int slot=(int)((now/w->tick_ms)%TW_SLOTS); for(int j=0;j<8&&c<cap;j++)if(w->slots[slot][j].active&&w->slots[slot][j].deadline<=now){out[c++]=w->slots[slot][j].id;w->slots[slot][j].active=0;} return c; }
