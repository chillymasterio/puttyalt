/* puttyalt_rewrap2.c - Recompute wrap offsets on resize.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
int rwp_lines_for(int textlen,int width){ if(width<=0)return 1; return textlen<=0?1:(textlen+width-1)/width; }
int rwp_total(const int *lengths,int n,int width){ int t=0; for(int i=0;i<n;i++)t+=rwp_lines_for(lengths[i],width); return t; }
void rwp_locate(int off,int width,int *line,int *col){ if(width<=0){if(line)*line=0;if(col)*col=off;return;} if(line)*line=off/width; if(col)*col=off%width; }
