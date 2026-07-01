/* puttyalt_portname.c - Well-known port to service name.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
const char *pnm_service(int port) {
    switch (port) {
        case 20: case 21: return "ftp";
        case 22: return "ssh";
        case 23: return "telnet";
        case 25: return "smtp";
        case 53: return "dns";
        case 80: return "http";
        case 110: return "pop3";
        case 143: return "imap";
        case 443: return "https";
        case 3306: return "mysql";
        case 5432: return "postgres";
        case 6379: return "redis";
        case 8080: return "http-alt";
        default: return "unknown";
    }
}
int pnm_is_wellknown(int port) { return port > 0 && port < 1024; }
int pnm_is_ephemeral(int port) { return port >= 49152 && port <= 65535; }
