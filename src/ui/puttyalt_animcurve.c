/* puttyalt_animcurve.c - Animation easing curves (cubic-bezier approximations). */
enum ac_easing { AC_LINEAR=0, AC_EASE_IN=1, AC_EASE_OUT=2, AC_EASE_IN_OUT=3, AC_BOUNCE=4, AC_ELASTIC=5 };
/* t in 0..1000 (fixed point), returns 0..1000. */
int animcurve_eval(int easing, int t) {
    if (t<0) t=0;
    if (t>1000) t=1000;
    double x=t/1000.0, y;
    switch(easing) {
        case AC_EASE_IN:     y=x*x; break;
        case AC_EASE_OUT:    y=1-(1-x)*(1-x); break;
        case AC_EASE_IN_OUT: y = x<0.5 ? 2*x*x : 1-2*(1-x)*(1-x); break;
        case AC_BOUNCE: {
            if (x<0.36) y=7.5625*x*x;
            else if (x<0.73) { x-=0.545; y=7.5625*x*x+0.75; }
            else if (x<0.9) { x-=0.818; y=7.5625*x*x+0.9375; }
            else { x-=0.955; y=7.5625*x*x+0.984; }
            break;
        }
        case AC_ELASTIC: { y = x==0?0:(x==1?1:1+ (x<1?-1:1)); if(x>0&&x<1){ y=1; /* simplified */ } break; }
        default: y=x; break;
    }
    int r=(int)(y*1000+0.5); if(r<0)r=0; if(r>1000)r=1000; return r;
}
int animcurve_lerp(int from, int to, int easing, int t) {
    int e=animcurve_eval(easing,t);
    return from + (to-from)*e/1000;
}
