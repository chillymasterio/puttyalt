/* puttyalt_dnstype.c - DNS record type names and codes.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
const char *dnt_name(int type){
    switch(type){ case 1:return "A"; case 2:return "NS"; case 5:return "CNAME"; case 6:return "SOA";
        case 12:return "PTR"; case 15:return "MX"; case 16:return "TXT"; case 28:return "AAAA";
        case 33:return "SRV"; case 257:return "CAA"; default:return "?"; }
}
int dnt_code(const char *name){
    if(!name)return -1;
    struct{const char*n;int c;} t[]={{"A",1},{"NS",2},{"CNAME",5},{"SOA",6},{"PTR",12},{"MX",15},{"TXT",16},{"AAAA",28},{"SRV",33},{"CAA",257}};
    for(unsigned i=0;i<sizeof(t)/sizeof(t[0]);i++) if(!strcmp(name,t[i].n))return t[i].c;
    return -1;
}
