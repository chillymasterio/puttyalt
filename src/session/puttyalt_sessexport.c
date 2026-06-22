/* puttyalt_sessexport.c - Export sessions to OpenSSH config format.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdio.h>
#include <string.h>
typedef struct { char name[48]; char host[96]; int port; char user[48]; char keyfile[128]; } ExportSession;
int se3_format(const ExportSession *s, char *out, int outlen) {
    if (!s || !out) return -1;
    int o = 0;
    o += snprintf(out+o, outlen-o, "Host %s\n", s->name);
    o += snprintf(out+o, outlen-o, "    HostName %s\n", s->host);
    if (s->port && s->port != 22) o += snprintf(out+o, outlen-o, "    Port %d\n", s->port);
    if (s->user[0]) o += snprintf(out+o, outlen-o, "    User %s\n", s->user);
    if (s->keyfile[0]) o += snprintf(out+o, outlen-o, "    IdentityFile %s\n", s->keyfile);
    return o;
}
