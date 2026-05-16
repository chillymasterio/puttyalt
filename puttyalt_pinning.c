/*
 * puttyalt_pinning.c: Host key pinning implementation.
 */

#include "puttyalt_pinning.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

void pinstore_init(PinStore *ps)
{
    memset(ps, 0, sizeof(*ps));
}

static int find_host(const PinStore *ps, const char *hostname, int port)
{
    for (int i = 0; i < ps->count; i++) {
        if (strcmp(ps->hosts[i].hostname, hostname) == 0 &&
            ps->hosts[i].port == port)
            return i;
    }
    return -1;
}

int pinstore_check(const PinStore *ps, const char *hostname, int port,
                   const char *fingerprint)
{
    int idx = find_host(ps, hostname, port);
    if (idx < 0)
        return -1;  /* not pinned, first connection */
    if (strcmp(ps->hosts[idx].fingerprint, fingerprint) == 0)
        return 0;   /* match */
    return 1;       /* MISMATCH — possible MITM */
}

int pinstore_add(PinStore *ps, const char *hostname, int port,
                 const char *fingerprint, const char *algo)
{
    int idx = find_host(ps, hostname, port);
    if (idx >= 0) {
        /* Update existing */
        snprintf(ps->hosts[idx].fingerprint, 128, "%s", fingerprint);
        if (algo)
            snprintf(ps->hosts[idx].algorithm, 32, "%s", algo);
        ps->hosts[idx].last_seen = (long)time(NULL);
        return idx;
    }

    if (ps->count >= MAX_PINNED_HOSTS)
        return -1;

    PinnedHost *h = &ps->hosts[ps->count];
    memset(h, 0, sizeof(*h));
    snprintf(h->hostname, 256, "%s", hostname);
    h->port = port;
    snprintf(h->fingerprint, 128, "%s", fingerprint);
    if (algo) snprintf(h->algorithm, 32, "%s", algo);
    h->first_seen = (long)time(NULL);
    h->last_seen = h->first_seen;
    h->trust_level = 0;

    return ps->count++;
}

int pinstore_remove(PinStore *ps, const char *hostname, int port)
{
    int idx = find_host(ps, hostname, port);
    if (idx < 0) return -1;
    for (int i = idx; i < ps->count - 1; i++)
        ps->hosts[i] = ps->hosts[i + 1];
    ps->count--;
    return 0;
}

int pinstore_load(PinStore *ps, const char *path)
{
    FILE *f = fopen(path, "r");
    char line[1024];
    if (!f) return -1;
    pinstore_init(ps);

    while (fgets(line, sizeof(line), f) && ps->count < MAX_PINNED_HOSTS) {
        char host[256], fp[128], algo[32];
        int port, trust;
        if (sscanf(line, "%255s %d %127s %31s %d", host, &port, fp, algo, &trust) >= 3) {
            PinnedHost *h = &ps->hosts[ps->count++];
            memset(h, 0, sizeof(*h));
            snprintf(h->hostname, 256, "%s", host);
            h->port = port;
            snprintf(h->fingerprint, 128, "%s", fp);
            snprintf(h->algorithm, 32, "%s", algo);
            h->trust_level = trust;
        }
    }
    fclose(f);
    return 0;
}

int pinstore_save(const PinStore *ps, const char *path)
{
    FILE *f = fopen(path, "w");
    if (!f) return -1;
    for (int i = 0; i < ps->count; i++) {
        const PinnedHost *h = &ps->hosts[i];
        fprintf(f, "%s %d %s %s %d\n",
                h->hostname, h->port, h->fingerprint,
                h->algorithm, h->trust_level);
    }
    fclose(f);
    return 0;
}
