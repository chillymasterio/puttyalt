/* puttyalt_c0names.c - Names for C0 control characters.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
const char *c0_name(int c) {
    static const char *names[] = {"NUL","SOH","STX","ETX","EOT","ENQ","ACK","BEL","BS","HT","LF","VT","FF","CR","SO","SI",
        "DLE","DC1","DC2","DC3","DC4","NAK","SYN","ETB","CAN","EM","SUB","ESC","FS","GS","RS","US"};
    if (c>=0 && c<32) return names[c];
    if (c==127) return "DEL";
    return "?";
}
int c0_is_control(int c) { return (c>=0&&c<32)||c==127; }
char c0_caret(int c) { if(c<32)return '@'+c; if(c==127)return '?'; return c; }
