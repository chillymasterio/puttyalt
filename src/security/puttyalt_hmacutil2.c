/* puttyalt_hmacutil2.c - HMAC block preparation for any hash.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
int hmk_prepare(const unsigned char *key,int keylen,int block,unsigned char *ipad,unsigned char *opad){
    if(!key||!ipad||!opad||block<=0||block>128)return -1;
    unsigned char k[128]; memset(k,0,block);
    if(keylen>block)keylen=block;
    memcpy(k,key,keylen);
    for(int i=0;i<block;i++){ ipad[i]=k[i]^0x36; opad[i]=k[i]^0x5c; }
    return 0;
}
