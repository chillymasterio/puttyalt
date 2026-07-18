/* puttyalt_dataunit.c - Data size unit conversions.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdio.h>
long dtu_to_bytes(double value,const char *unit){
    if(!unit)return (long)value;
    switch(unit[0]){ case 'K':case 'k':return (long)(value*1024); case 'M':case 'm':return (long)(value*1024*1024); case 'G':case 'g':return (long)(value*1024*1024*1024); default:return (long)value; }
}
int dtu_format(long bytes,char *out,int outlen){
    const char *u[]={"B","KiB","MiB","GiB","TiB"}; double v=(double)bytes; int i=0;
    while(v>=1024&&i<4){v/=1024;i++;}
    return i==0?snprintf(out,outlen,"%ld %s",bytes,u[0]):snprintf(out,outlen,"%.2f %s",v,u[i]);
}
