/* puttyalt_certexpiry.c - Certificate expiry monitoring + warnings. */
#include <string.h>
#include <stdio.h>
#define CE_MAX 32
#define CE_NAME 96
typedef struct {
    char name[CE_NAME];
    long expires_epoch;
    int warned;
} ce_cert;
typedef struct {
    ce_cert certs[CE_MAX];
    int n;
    int warn_days;
} CertExpiry;
void certexpiry_init(CertExpiry *c, int warn_days) {
    if (!c) return;
    memset(c, 0, sizeof(*c));
    c->warn_days = warn_days > 0 ? warn_days : 30;
}
int certexpiry_add(CertExpiry *c, const char *name, long expires_epoch) {
    if (!c || c->n >= CE_MAX || !name) return -1;
    snprintf(c->certs[c->n].name, CE_NAME, "%s", name);
    c->certs[c->n].expires_epoch = expires_epoch;
    return c->n++;
}
int certexpiry_days_left(const CertExpiry *c, int idx, long now_epoch) {
    if (!c || idx < 0 || idx >= c->n) return -1;
    long diff = c->certs[idx].expires_epoch - now_epoch;
    return (int)(diff / 86400);
}
int certexpiry_expiring(CertExpiry *c, long now_epoch, int *out_idx, int cap) {
    if (!c) return -1;
    int n = 0;
    for (int i = 0; i < c->n && n < cap; i++) {
        int days = certexpiry_days_left(c, i, now_epoch);
        if (days <= c->warn_days) {
            out_idx[n++] = i;
            c->certs[i].warned = 1;
        }
    }
    return n;
}
int certexpiry_is_expired(const CertExpiry *c, int idx, long now_epoch) {
    if (!c || idx < 0 || idx >= c->n) return -1;
    return c->certs[idx].expires_epoch <= now_epoch ? 1 : 0;
}
int certexpiry_count(const CertExpiry *c) {
    return c ? c->n : -1;
}
