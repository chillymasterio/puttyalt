/* puttyalt_dnssec.c - DNSSEC validation state (chain of trust signaling). */
#include <string.h>
#include <stdio.h>
enum ds_status { DS_INSECURE=0, DS_SECURE=1, DS_BOGUS=2, DS_INDETERMINATE=3 };
typedef struct { char domain[160]; int status; int has_rrsig; int has_ds; int chain_depth; } DnsSec;
void dnssec_init(DnsSec *d, const char *domain) {
    if(!d) return; memset(d,0,sizeof(*d)); snprintf(d->domain,160,"%s",domain?domain:"");
    d->status=DS_INDETERMINATE;
}
void dnssec_set_records(DnsSec *d, int has_rrsig, int has_ds) { if(d){ d->has_rrsig=has_rrsig?1:0; d->has_ds=has_ds?1:0; } }
int dnssec_validate(DnsSec *d, int signatures_valid) {
    if(!d) return -1;
    if (!d->has_rrsig && !d->has_ds) { d->status=DS_INSECURE; return DS_INSECURE; }
    if (signatures_valid) { d->status=DS_SECURE; return DS_SECURE; }
    d->status=DS_BOGUS; return DS_BOGUS;
}
int dnssec_is_trusted(const DnsSec *d) { return (d&&d->status==DS_SECURE)?1:0; }
int dnssec_status(const DnsSec *d) { return d?d->status:-1; }
