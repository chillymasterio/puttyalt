/* puttyalt_sshver.c - Parse SSH version-exchange banners.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#include <stdio.h>
typedef struct { char proto[8]; char software[64]; char comment[64]; } SshBanner;
int sshv_parse(const char *line, SshBanner *b) {
    if (!line || !b) return -1;
    if (strncmp(line, "SSH-", 4) != 0) return -1;
    b->proto[0] = b->software[0] = b->comment[0] = 0;
    const char *p = line + 4;
    int i = 0;
    while (*p && *p != '-' && i < 7) b->proto[i++] = *p++;
    b->proto[i] = 0;
    if (*p != '-') return -1;
    p++;
    i = 0;
    while (*p && *p != ' ' && *p != '\r' && *p != '\n' && i < 63) b->software[i++] = *p++;
    b->software[i] = 0;
    if (*p == ' ') {
        p++;
        i = 0;
        while (*p && *p != '\r' && *p != '\n' && i < 63) b->comment[i++] = *p++;
        b->comment[i] = 0;
    }
    return 0;
}
int sshv_is_v2(const SshBanner *b) { return b && strcmp(b->proto, "2.0") == 0; }
