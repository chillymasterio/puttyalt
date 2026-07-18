/* puttyalt_rgbhsl.c - RGB to HSL conversion.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
void rhl_to_hsl(uint8_t r,uint8_t g,uint8_t b,int *h,int *s,int *l){
    int mx=r>g?(r>b?r:b):(g>b?g:b), mn=r<g?(r<b?r:b):(g<b?g:b);
    int d=mx-mn;
    if(l)*l=(mx+mn)*100/510;
    if(d==0){ if(h)*h=0; if(s)*s=0; return; }
    if(s){ int sum=mx+mn; *s=(sum<=255)?d*100/sum:d*100/(510-sum); }
    if(h){ int hue; if(mx==r)hue=60*(((g-b)*100/d))/100; else if(mx==g)hue=60*(2+(b-r)*100/d/100); else hue=60*(4+(r-g)*100/d/100); if(hue<0)hue+=360; *h=hue; }
}
