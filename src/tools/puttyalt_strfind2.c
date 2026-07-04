/* puttyalt_strfind2.c - Find nth occurrence and last match.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
int sfd_nth(const char *s, const char *sub, int nth){
    if(!s||!sub||nth<1)return -1;
    int sl=(int)strlen(sub),count=0;
    for(const char *p=s;(p=strstr(p,sub));p+=sl){ if(++count==nth)return (int)(p-s); }
    return -1;
}
int sfd_last(const char *s, const char *sub){
    if(!s||!sub||!*sub)return -1;
    int sl=(int)strlen(sub),last=-1;
    for(const char *p=s;(p=strstr(p,sub));p+=sl)last=(int)(p-s);
    return last;
}
