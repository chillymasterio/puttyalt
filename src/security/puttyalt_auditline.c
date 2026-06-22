/* puttyalt_auditline.c - Format tamper-evident audit log lines.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdio.h>
#include <stdint.h>
/* Chain hash: combine previous hash with a new entry deterministically. */
uint32_t au2_chain(uint32_t prev, const char *entry) {
    uint32_t h = prev ^ 2166136261u;
    for (const char *p = entry; p && *p; p++) { h ^= (unsigned char)*p; h *= 16777619u; }
    return h;
}
int au2_format(long timestamp, const char *user, const char *action, uint32_t chainhash, char *out, int outlen) {
    return snprintf(out, outlen, "%ld|%s|%s|%08x", timestamp, user ? user : "?", action ? action : "?", chainhash);
}
/* Verify a chain by recomputing from a known-good previous hash. */
int au2_verify(uint32_t prev, const char *entry, uint32_t expected) {
    return au2_chain(prev, entry) == expected;
}
