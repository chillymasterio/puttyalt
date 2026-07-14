/* puttyalt_bitcount.c - Bit manipulation utilities.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
int btc_popcount(uint32_t x){ int c=0; while(x){c+=x&1;x>>=1;} return c; }
int btc_leading_zeros(uint32_t x){ if(x==0)return 32; int n=0; while(!(x&0x80000000u)){n++;x<<=1;} return n; }
int btc_trailing_zeros(uint32_t x){ if(x==0)return 32; int n=0; while(!(x&1)){n++;x>>=1;} return n; }
uint32_t btc_reverse(uint32_t x){ uint32_t r=0; for(int i=0;i<32;i++){r=(r<<1)|(x&1);x>>=1;} return r; }
