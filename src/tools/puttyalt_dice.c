/* puttyalt_dice.c - Deterministic dice/random ranges.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
int dic_roll(int sides,uint32_t *seed){ if(sides<1||!seed)return 0; *seed=*seed*1103515245u+12345u; return 1+(int)((*seed>>16)%sides); }
int dic_range(int lo,int hi,uint32_t *seed){ if(!seed||hi<lo)return lo; *seed=*seed*1103515245u+12345u; return lo+(int)((*seed>>16)%(hi-lo+1)); }
int dic_percent(uint32_t *seed){ return dic_range(0,100,seed); }
