/* puttyalt_otpuri.c - Build otpauth:// provisioning URIs.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdio.h>
int otu_build(const char *issuer,const char *account,const char *secret,int digits,int period,char *out,int outlen){
    if(!issuer||!account||!secret||!out)return -1;
    return snprintf(out,outlen,"otpauth://totp/%s:%s?secret=%s&issuer=%s&digits=%d&period=%d",
        issuer,account,secret,issuer,digits>0?digits:6,period>0?period:30);
}
