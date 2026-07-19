/* puttyalt_welcomecard.c - Format a welcome/summary card.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdio.h>
int wlc_format(const char *user,int sessions,int total_modules,char *out,int outlen){
    if(!out)return -1;
    return snprintf(out,outlen,"Welcome, %s | %d sessions | %d modules",user?user:"user",sessions,total_modules);
}
