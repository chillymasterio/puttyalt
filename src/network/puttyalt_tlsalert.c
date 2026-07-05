/* puttyalt_tlsalert.c - TLS alert code descriptions.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
const char *tla_desc(int code){
    switch(code){ case 0:return "close_notify"; case 10:return "unexpected_message"; case 20:return "bad_record_mac";
        case 40:return "handshake_failure"; case 42:return "bad_certificate"; case 43:return "unsupported_certificate";
        case 44:return "certificate_revoked"; case 45:return "certificate_expired"; case 46:return "certificate_unknown";
        case 48:return "unknown_ca"; case 50:return "decode_error"; case 51:return "decrypt_error";
        case 70:return "protocol_version"; case 80:return "internal_error"; case 90:return "user_canceled"; default:return "unknown"; }
}
int tla_is_fatal(int code){ return code!=0 && code!=90 && code!=100; }
