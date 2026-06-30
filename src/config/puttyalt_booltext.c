/* puttyalt_booltext.c - Parse boolean values from text.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
int bt3_parse(const char *s, int dflt) {
    if (!s) return dflt;
    if (!strcasecmp(s,"true")||!strcasecmp(s,"yes")||!strcasecmp(s,"on")||!strcmp(s,"1")||!strcasecmp(s,"y")) return 1;
    if (!strcasecmp(s,"false")||!strcasecmp(s,"no")||!strcasecmp(s,"off")||!strcmp(s,"0")||!strcasecmp(s,"n")) return 0;
    return dflt;
}
const char *bt3_format(int value) { return value ? "true" : "false"; }
const char *bt3_onoff(int value) { return value ? "on" : "off"; }
const char *bt3_yesno(int value) { return value ? "yes" : "no"; }
