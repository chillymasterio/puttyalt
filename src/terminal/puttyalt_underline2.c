/* puttyalt_underline2.c - Underline style codes (SGR 4:x).
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
enum { UL_NONE, UL_SINGLE, UL_DOUBLE, UL_CURLY, UL_DOTTED, UL_DASHED };
int ul_from_param(int param){ switch(param){ case 0:return UL_NONE; case 1:return UL_SINGLE; case 2:return UL_DOUBLE; case 3:return UL_CURLY; case 4:return UL_DOTTED; case 5:return UL_DASHED; default:return UL_SINGLE; } }
const char *ul_name(int style){ static const char *n[]={"none","single","double","curly","dotted","dashed"}; return (style>=0&&style<=UL_DASHED)?n[style]:"none"; }
