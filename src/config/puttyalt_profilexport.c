/* puttyalt_profilexport.c - Export/import full profile as portable INI blob. */
#include <string.h>
#include <stdio.h>
#define PE_MAX 64
#define PE_K 48
#define PE_V 192
typedef struct { char section[PE_K]; char key[PE_K]; char val[PE_V]; } pe_row;
typedef struct { pe_row r[PE_MAX]; int n; } ProfileExport;
void profilexport_init(ProfileExport *p) { if(p) memset(p,0,sizeof(*p)); }
int profilexport_add(ProfileExport *p, const char *section, const char *key, const char *val) {
    if(!p||p->n>=PE_MAX) return -1;
    snprintf(p->r[p->n].section,PE_K,"%s",section?section:"default");
    snprintf(p->r[p->n].key,PE_K,"%s",key?key:"");
    snprintf(p->r[p->n].val,PE_V,"%s",val?val:""); p->n++; return 0;
}
int profilexport_write(const ProfileExport *p, char *buf, int buflen) {
    if(!p||!buf) return -1;
    int pos=0; char cur[PE_K]=""; 
    for (int i=0;i<p->n && pos<buflen;i++) {
        if (strcmp(cur,p->r[i].section)!=0) { snprintf(cur,PE_K,"%s",p->r[i].section); pos+=snprintf(buf+pos,buflen-pos,"[%s]\n",cur); }
        pos+=snprintf(buf+pos,buflen-pos,"%s=%s\n",p->r[i].key,p->r[i].val);
    }
    return pos;
}
int profilexport_count(const ProfileExport *p) { return p?p->n:-1; }
