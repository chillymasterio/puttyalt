/* puttyalt_certpin.c - Certificate pinning store (SPKI hash allowlist + report). */
#include <string.h>
#include <stdio.h>
#define CT_MAX 32
#define CT_HOST 96
#define CT_PIN 64
typedef struct { char host[CT_HOST]; char pin[CT_PIN]; int backup; } ct_pin;
typedef struct { ct_pin pins[CT_MAX]; int n; int enforce; } CertPin;
void certpin_init(CertPin *c, int enforce) { if(c){ memset(c,0,sizeof(*c)); c->enforce=enforce?1:0; } }
int certpin_add(CertPin *c, const char *host, const char *spki_hash, int is_backup) {
    if(!c||c->n>=CT_MAX||!host||!spki_hash) return -1;
    ct_pin *p=&c->pins[c->n++]; snprintf(p->host,CT_HOST,"%s",host); snprintf(p->pin,CT_PIN,"%s",spki_hash); p->backup=is_backup?1:0; return 0;
}
int certpin_verify(const CertPin *c, const char *host, const char *presented_hash) {
    if(!c||!host||!presented_hash) return -1;
    int have_pins=0, match=0;
    for (int i=0;i<c->n;i++) if (strcmp(c->pins[i].host,host)==0) {
        have_pins=1; if (strcmp(c->pins[i].pin,presented_hash)==0) match=1;
    }
    if (!have_pins) return 1; /* no pins for host = pass-through */
    if (match) return 0; /* pinned & matches */
    return c->enforce ? -1 : 1; /* mismatch: block if enforcing, else allow+report */
}
int certpin_host_pinned(const CertPin *c, const char *host) {
    if(!c||!host) return 0;
    for (int i=0;i<c->n;i++) if (strcmp(c->pins[i].host,host)==0) return 1;
    return 0;
}
int certpin_count(const CertPin *c) { return c?c->n:-1; }
