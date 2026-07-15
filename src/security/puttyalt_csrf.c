/* puttyalt_csrf.c - CSRF token generation and check.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
#include <string.h>
uint64_t csf_generate(uint64_t session_seed,uint64_t nonce){ uint64_t h=session_seed^nonce; h^=h>>33; h*=0xff51afd7ed558ccdULL; h^=h>>33; return h; }
int csf_verify(uint64_t expected,uint64_t provided){ uint64_t d=expected^provided; return d==0; }
