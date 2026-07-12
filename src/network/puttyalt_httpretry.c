/* puttyalt_httpretry.c - HTTP retry decision from status.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
int htr_should_retry(int status,int attempt,int max_attempts){
    if(attempt>=max_attempts)return 0;
    if(status==429||status==503||status==502||status==504)return 1;
    if(status>=500&&status<600)return 1;
    return 0;
}
long htr_delay(int attempt,long base){ long d=base; for(int i=0;i<attempt;i++)d*=2; return d>60?60:d; }
