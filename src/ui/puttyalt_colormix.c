/* puttyalt_colormix.c - Blend and adjust RGB colors.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
uint32_t cm3_blend(uint32_t a, uint32_t b, int alpha) {
    if (alpha < 0) alpha = 0;
    if (alpha > 255) alpha = 255;
    int ra=(a>>16)&255, ga=(a>>8)&255, ba=a&255;
    int rb=(b>>16)&255, gb=(b>>8)&255, bb=b&255;
    int r=(ra*(255-alpha)+rb*alpha)/255;
    int g=(ga*(255-alpha)+gb*alpha)/255;
    int bl=(ba*(255-alpha)+bb*alpha)/255;
    return (uint32_t)((r<<16)|(g<<8)|bl);
}
uint32_t cm3_lighten(uint32_t c, int amount) {
    int r=(c>>16)&255, g=(c>>8)&255, b=c&255;
    r+=amount; g+=amount; b+=amount;
    if(r>255)r=255;
    if(g>255)g=255;
    if(b>255)b=255;
    if(r<0)r=0;
    if(g<0)g=0;
    if(b<0)b=0;
    return (uint32_t)((r<<16)|(g<<8)|b);
}
int cm3_contrast_ratio_x10(uint32_t fg, uint32_t bg) {
    int lf=((fg>>16&255)*299+(fg>>8&255)*587+(fg&255)*114)/1000;
    int lb=((bg>>16&255)*299+(bg>>8&255)*587+(bg&255)*114)/1000;
    int hi=lf>lb?lf:lb, lo=lf>lb?lb:lf;
    return (hi*10)/(lo+1);
}
