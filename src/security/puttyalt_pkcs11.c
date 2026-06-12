/* puttyalt_pkcs11.c - PKCS#11 hardware token slot descriptor (smartcard/HSM). */
#include <string.h>
#include <stdio.h>
#define PK_MAX 8
#define PK_NAME 64
typedef struct { char label[PK_NAME]; char manufacturer[48]; int slot_id; int has_token; int key_count; int login_required; } pk_slot;
typedef struct { pk_slot slots[PK_MAX]; int n; char module_path[160]; } Pkcs11;
void pkcs11_init(Pkcs11 *p, const char *module_path) {
    if(!p) return;
    memset(p,0,sizeof(*p)); snprintf(p->module_path,160,"%s",module_path?module_path:"");
}
int pkcs11_add_slot(Pkcs11 *p, int slot_id, const char *label, int has_token) {
    if(!p||p->n>=PK_MAX) return -1;
    pk_slot *s=&p->slots[p->n]; s->slot_id=slot_id; snprintf(s->label,PK_NAME,"%s",label?label:""); s->has_token=has_token?1:0;
    return p->n++;
}
int pkcs11_find_token(const Pkcs11 *p) {
    if(!p) return -1;
    for(int i=0;i<p->n;i++) if(p->slots[i].has_token) return i; return -1;
}
void pkcs11_set_keys(Pkcs11 *p, int idx, int key_count, int login_required) {
    if(!p||idx<0||idx>=p->n) return;
    p->slots[idx].key_count=key_count; p->slots[idx].login_required=login_required?1:0;
}
int pkcs11_slot_count(const Pkcs11 *p) { return p?p->n:-1; }
int pkcs11_total_keys(const Pkcs11 *p) {
    if(!p) return -1;
    int n=0; for(int i=0;i<p->n;i++) n+=p->slots[i].key_count; return n;
}
