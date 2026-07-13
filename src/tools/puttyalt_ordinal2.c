/* puttyalt_ordinal2.c - Ordinal suffixes (1st, 2nd, 3rd).
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdio.h>
const char *ord_suffix(int n){ int m=n%100; if(m>=11&&m<=13)return "th"; switch(n%10){case 1:return "st";case 2:return "nd";case 3:return "rd";default:return "th";} }
int ord_format(int n,char *out,int outlen){ return snprintf(out,outlen,"%d%s",n,ord_suffix(n)); }
