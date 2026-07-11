/* puttyalt_slider.c - Slider widget with range and step.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { int value; int min; int max; int step; } Slider;
void sld_init(Slider *s,int min,int max,int step){ if(s){s->min=min;s->max=max;s->step=step>0?step:1;s->value=min;} }
void sld_inc(Slider *s){ if(!s)return; s->value+=s->step; if(s->value>s->max)s->value=s->max; }
void sld_dec(Slider *s){ if(!s)return; s->value-=s->step; if(s->value<s->min)s->value=s->min; }
int sld_percent(const Slider *s){ if(!s||s->max==s->min)return 0; return (s->value-s->min)*100/(s->max-s->min); }
