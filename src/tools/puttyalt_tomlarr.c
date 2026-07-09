/* puttyalt_tomlarr.c - Parse TOML inline arrays.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <ctype.h>
#include <stddef.h>
/* Count elements in a TOML inline array "[a, b, c]". */
int tar_count(const char *s){
    if(!s)return 0;
    while(*s==' ')s++;
    if(*s!='[')return -1;
    s++;
    int count=0,depth=0,seen=0,instr=0;
    for(;*s;s++){ if(instr){if(*s=='"')instr=0;continue;} if(*s=='"'){instr=1;seen=1;} else if(*s=='[')depth++; else if(*s==']'){if(depth==0){return seen?count+1:0;}depth--;} else if(*s==','&&depth==0)count++; else if(!isspace((unsigned char)*s))seen=1; }
    return -1;
}
