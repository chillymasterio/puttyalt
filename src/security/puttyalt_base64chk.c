/* puttyalt_base64chk.c - Validate base64 strings.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
static int b64c_valid_char(char c){ return (c>='A'&&c<='Z')||(c>='a'&&c<='z')||(c>='0'&&c<='9')||c=='+'||c=='/'||c=='='; }
int b64c_is_valid(const char *s){
    if(!s)return 0;
    int len=0,pad=0;
    for(const char *p=s;*p;p++){ if(!b64c_valid_char(*p))return 0; if(*p=='=')pad++; len++; }
    return len%4==0&&pad<=2;
}
