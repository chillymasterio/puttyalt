/* puttyalt_crc32.c - CRC32 checksum (IEEE 802.3). */
#include <stdint.h>
static uint32_t crc_table[256];
static int crc_init_done=0;
static void crc32_build(void) {
    for (uint32_t i=0;i<256;i++) {
        uint32_t c=i;
        for (int k=0;k<8;k++) c = (c&1) ? 0xEDB88320u^(c>>1) : c>>1;
        crc_table[i]=c;
    }
    crc_init_done=1;
}
uint32_t crc32_update(uint32_t crc, const unsigned char *data, int len) {
    if (!crc_init_done) crc32_build();
    crc=~crc;
    for (int i=0;i<len;i++) crc=crc_table[(crc^data[i])&0xFF]^(crc>>8);
    return ~crc;
}
uint32_t crc32_compute(const unsigned char *data, int len) {
    return crc32_update(0, data, len);
}
int crc32_verify(const unsigned char *data, int len, uint32_t expected) {
    return crc32_compute(data,len)==expected ? 1 : 0;
}
