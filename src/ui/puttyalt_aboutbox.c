/* puttyalt_aboutbox.c - Format application about information.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdio.h>
int abt_format(const char *name,const char *version,const char *license,char *out,int outlen){
    if(!name||!out)return -1;
    return snprintf(out,outlen,"%s %s\n%s License",name,version?version:"",license?license:"MIT");
}
