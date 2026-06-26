/* puttyalt_statusicon.c - Map connection state to status icons.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
enum { ST_DISCONNECTED, ST_CONNECTING, ST_CONNECTED, ST_ERROR, ST_IDLE };
const char *si3_icon(int state) {
    switch (state) {
        case ST_CONNECTED: return "*";
        case ST_CONNECTING: return "~";
        case ST_ERROR: return "!";
        case ST_IDLE: return "-";
        default: return "o";
    }
}
const char *si3_label(int state) {
    switch (state) {
        case ST_CONNECTED: return "Connected";
        case ST_CONNECTING: return "Connecting";
        case ST_ERROR: return "Error";
        case ST_IDLE: return "Idle";
        default: return "Disconnected";
    }
}
int si3_color(int state) {
    switch (state) {
        case ST_CONNECTED: return 0x3FB950;
        case ST_CONNECTING: return 0xD29922;
        case ST_ERROR: return 0xF85149;
        default: return 0x8B949E;
    }
}
