/* puttyalt_sftpattr.c - SFTP file attributes parser/formatter (permissions, times). */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
typedef struct { uint64_t size; uint32_t uid, gid; uint32_t permissions; uint64_t atime, mtime; int is_dir, is_link; } SftpAttr;
void sftpattr_init(SftpAttr *a) { if(a) memset(a,0,sizeof(*a)); }
void sftpattr_set_perms(SftpAttr *a, uint32_t perms) {
    if(!a) return; a->permissions=perms;
    a->is_dir = (perms & 0040000)?1:0;
    a->is_link = ((perms & 0170000)==0120000)?1:0;
}
int sftpattr_format_mode(const SftpAttr *a, char *out, int outlen) {
    if(!a||!out||outlen<11) return -1;
    uint32_t p=a->permissions;
    out[0] = a->is_dir?'d':(a->is_link?'l':'-');
    const char *rwx="rwxrwxrwx";
    for (int i=0;i<9;i++) out[1+i] = (p & (1<<(8-i))) ? rwx[i] : '-';
    out[10]=0;
    return 10;
}
int sftpattr_format_size(const SftpAttr *a, char *out, int outlen) {
    if(!a||!out) return -1;
    double s=(double)a->size; const char *u[]={"B","K","M","G","T"}; int i=0;
    while (s>=1024 && i<4) { s/=1024; i++; }
    if (i==0) return snprintf(out,outlen,"%llu",(unsigned long long)a->size);
    return snprintf(out,outlen,"%.1f%s",s,u[i]);
}
int sftpattr_is_executable(const SftpAttr *a) { return (a&&(a->permissions&0111))?1:0; }
