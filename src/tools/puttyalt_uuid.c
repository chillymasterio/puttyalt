/* puttyalt_uuid.c - UUID v4-style generator (deterministic from seed) + parser. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
int uuid_generate(uint32_t *seed, char *out, int outlen) {
    if(!seed||!out||outlen<37) return -1;
    unsigned char b[16];
    for (int i=0;i<16;i++) { *seed = (*seed)*1103515245u + 12345u; b[i]=(unsigned char)((*seed>>16)&0xFF); }
    b[6]=(b[6]&0x0F)|0x40; /* version 4 */
    b[8]=(b[8]&0x3F)|0x80; /* variant */
    return snprintf(out,outlen,"%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
        b[0],b[1],b[2],b[3],b[4],b[5],b[6],b[7],b[8],b[9],b[10],b[11],b[12],b[13],b[14],b[15]);
}
int uuid_is_valid(const char *s) {
    if(!s||strlen(s)!=36) return 0;
    for (int i=0;i<36;i++) {
        if (i==8||i==13||i==18||i==23) { if(s[i]!='-') return 0; }
        else { char c=s[i]; if(!((c>='0'&&c<='9')||(c>='a'&&c<='f')||(c>='A'&&c<='F'))) return 0; }
    }
    return 1;
}
int uuid_version(const char *s) {
    if(!uuid_is_valid(s)) return -1;
    char v=s[14]; return (v>='0'&&v<='9')?v-'0':((v>='a'&&v<='f')?v-'a'+10:-1);
}
