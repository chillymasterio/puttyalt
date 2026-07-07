/* puttyalt_checksum16.c - 16-bit checksums (BSD/Internet).
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
uint16_t cs16_bsd(const unsigned char *d,int n){ uint16_t c=0; for(int i=0;i<n;i++){ c=(c>>1)|(c<<15); c+=d[i]; } return c; }
uint16_t cs16_internet(const unsigned char *d,int n){ uint32_t sum=0; for(int i=0;i+1<n;i+=2)sum+=(d[i]<<8)|d[i+1]; if(n&1)sum+=d[n-1]<<8; while(sum>>16)sum=(sum&0xFFFF)+(sum>>16); return (uint16_t)~sum; }
