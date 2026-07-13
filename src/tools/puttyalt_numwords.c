/* puttyalt_numwords.c - Convert small integers to English words.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
static const char *NW_ONES[]={"zero","one","two","three","four","five","six","seven","eight","nine","ten",
"eleven","twelve","thirteen","fourteen","fifteen","sixteen","seventeen","eighteen","nineteen"};
static const char *NW_TENS[]={"","","twenty","thirty","forty","fifty","sixty","seventy","eighty","ninety"};
int nwd_convert(int n,char *out,int outlen){
    if(!out)return -1;
    if(n<0){ int r=nwd_convert(-n,out+6,outlen-6); memcpy(out,"minus ",6); return r+6; }
    if(n<20){ strncpy(out,NW_ONES[n],outlen-1); out[outlen-1]=0; return (int)strlen(out); }
    if(n<100){ int o=0; const char *t=NW_TENS[n/10]; while(*t&&o<outlen-1)out[o++]=*t++; if(n%10){if(o<outlen-1)out[o++]='-'; const char *u=NW_ONES[n%10]; while(*u&&o<outlen-1)out[o++]=*u++;} out[o]=0; return o; }
    strncpy(out,"many",outlen-1); out[outlen-1]=0; return 4;
}
