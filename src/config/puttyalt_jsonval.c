/* puttyalt_jsonval.c - Detect JSON value types.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <ctype.h>
#include <stddef.h>
enum { JV_NULL, JV_BOOL, JV_NUMBER, JV_STRING, JV_ARRAY, JV_OBJECT, JV_INVALID };
int jvl_type(const char *s){
    if(!s)return JV_INVALID;
    while(*s==' ')s++;
    switch(*s){ case '"':return JV_STRING; case '{':return JV_OBJECT; case '[':return JV_ARRAY;
        case 't': case 'f':return JV_BOOL; case 'n':return JV_NULL;
        default: if(*s=='-'||isdigit((unsigned char)*s))return JV_NUMBER; return JV_INVALID; }
}
