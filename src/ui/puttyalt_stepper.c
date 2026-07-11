/* puttyalt_stepper.c - Numeric stepper widget.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { int value; int min; int max; int wrap; } Stepper;
void stp_init(Stepper *s,int min,int max,int wrap){ if(s){s->min=min;s->max=max;s->wrap=wrap;s->value=min;} }
void stp_up(Stepper *s){ if(!s)return; if(s->value<s->max)s->value++; else if(s->wrap)s->value=s->min; }
void stp_down(Stepper *s){ if(!s)return; if(s->value>s->min)s->value--; else if(s->wrap)s->value=s->max; }
