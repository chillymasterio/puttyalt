/* puttyalt_crc32b.c - CRC-32 (bitwise) checksum.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
uint32_t crc32b_compute(const unsigned char *d,int n){ uint32_t crc=0xFFFFFFFF; for(int i=0;i<n;i++){ crc^=d[i]; for(int k=0;k<8;k++) crc=(crc>>1)^(0xEDB88320&(-(int)(crc&1))); } return ~crc; }
uint32_t crc32b_update(uint32_t crc,unsigned char b){ crc^=b; for(int k=0;k<8;k++)crc=(crc>>1)^(0xEDB88320&(-(int)(crc&1))); return crc; }
