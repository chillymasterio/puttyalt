/* puttyalt_colorwheel.c - HSV color wheel picker + RGB conversion. */
#include <stdint.h>
uint32_t colorwheel_hsv_to_rgb(int h, int s, int v) {
    /* h: 0-359, s: 0-100, v: 0-100 */
    if (s==0) { int g=v*255/100; return (g<<16)|(g<<8)|g; }
    int region=h/60; int rem=(h%60)*255/60;
    int p=v*(100-s)*255/10000;
    int q=v*(100-(s*rem/255))*255/10000;
    int t=v*(100-(s*(255-rem)/255))*255/10000;
    int vv=v*255/100;
    int r,g,b;
    switch(region){
        case 0: r=vv;g=t;b=p; break; case 1: r=q;g=vv;b=p; break;
        case 2: r=p;g=vv;b=t; break; case 3: r=p;g=q;b=vv; break;
        case 4: r=t;g=p;b=vv; break; default: r=vv;g=p;b=q; break;
    }
    return ((r&0xFF)<<16)|((g&0xFF)<<8)|(b&0xFF);
}
void colorwheel_rgb_to_hsv(uint32_t rgb, int *h, int *s, int *v) {
    int r=(rgb>>16)&0xFF, g=(rgb>>8)&0xFF, b=rgb&0xFF;
    int max=r>g?(r>b?r:b):(g>b?g:b);
    int min=r<g?(r<b?r:b):(g<b?g:b);
    int delta=max-min;
    if (v) *v=max*100/255;
    if (s) *s=max==0?0:delta*100/max;
    if (h) {
        if (delta==0) *h=0;
        else if (max==r) *h=(60*(g-b)/delta+360)%360;
        else if (max==g) *h=(60*(b-r)/delta+120)%360;
        else *h=(60*(r-g)/delta+240)%360;
    }
}
