/* puttyalt_sftpresume.c - Resumable SFTP transfer offset tracking + checksum window. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define SR_PATH 256
typedef struct { char path[SR_PATH]; uint64_t offset; uint64_t total; uint32_t partial_crc; int complete; } SftpResume;
void sftpresume_init(SftpResume *r, const char *path, uint64_t total) {
    if(!r) return; memset(r,0,sizeof(*r)); snprintf(r->path,SR_PATH,"%s",path?path:""); r->total=total;
}
int sftpresume_advance(SftpResume *r, const unsigned char *chunk, int len) {
    if(!r||!chunk||len<0) return -1;
    for (int i=0;i<len;i++) { r->partial_crc = (r->partial_crc>>8) ^ ((r->partial_crc ^ chunk[i]) & 0xFF); }
    r->offset+=len;
    if (r->offset>=r->total && r->total>0) r->complete=1;
    return 0;
}
uint64_t sftpresume_offset(const SftpResume *r) { return r?r->offset:0; }
int sftpresume_pct(const SftpResume *r) { return (r&&r->total)? (int)((r->offset*100)/r->total):0; }
int sftpresume_is_complete(const SftpResume *r) { return r?r->complete:-1; }
int sftpresume_can_resume(const SftpResume *r, uint64_t server_size) {
    if(!r) return 0; return (server_size>0 && server_size<=r->total && server_size==r->offset)?1:0;
}
