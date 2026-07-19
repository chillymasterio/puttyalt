/* puttyalt_changelogp.c - Parse changelog version headers.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#include <ctype.h>
/* Detect a "## [x.y.z]" markdown changelog header; extract version. */
int chl_version(const char *line,char *out,int outlen){
    if(!line||!out)return 0;
    while(*line=='#'||*line==' ')line++;
    if(*line=='[')line++;
    int o=0;
    while(*line&&(isdigit((unsigned char)*line)||*line=='.')&&o<outlen-1)out[o++]=*line++;
    out[o]=0;
    return o>0;
}
