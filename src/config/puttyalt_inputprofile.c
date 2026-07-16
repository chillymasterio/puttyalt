/* puttyalt_inputprofile.c - Input handling profile settings.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { int backspace_del; int alt_sends_esc; int scroll_lines; int paste_bracketed; } InputProfile;
void ipf_defaults(InputProfile *p){ if(!p)return; p->backspace_del=1; p->alt_sends_esc=1; p->scroll_lines=3; p->paste_bracketed=1; }
int ipf_backspace_code(const InputProfile *p){ return (p&&p->backspace_del)?0x7f:0x08; }
