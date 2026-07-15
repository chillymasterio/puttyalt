/* puttyalt_pwbreach.c - Check password against common-pattern heuristics.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#include <ctype.h>
int pwb_is_common(const char *pw){
    if(!pw)return 1;
    static const char *common[]={"password","123456","qwerty","admin","letmein","welcome","111111"};
    for(unsigned i=0;i<sizeof(common)/sizeof(common[0]);i++)if(!strcasecmp(pw,common[i]))return 1;
    return 0;
}
int pwb_only_digits(const char *pw){ if(!pw||!*pw)return 0; for(;*pw;pw++)if(!isdigit((unsigned char)*pw))return 0; return 1; }
