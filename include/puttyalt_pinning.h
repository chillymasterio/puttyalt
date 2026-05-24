/*
 * puttyalt_pinning.h: SSH host key pinning beyond TOFU.
 *
 * Records expected host key fingerprints and alerts if they change
 * unexpectedly, providing defence against MITM attacks.
 */

#ifndef PUTTYALT_PINNING_H
#define PUTTYALT_PINNING_H

#define MAX_PINNED_HOSTS 512

typedef struct PinnedHost {
    char hostname[256];
    int  port;
    char fingerprint[128];
    char algorithm[32];     /* ssh-ed25519, ssh-rsa, etc. */
    long first_seen;
    long last_seen;
    int  trust_level;       /* 0=tofu, 1=verified, 2=pinned */
} PinnedHost;

typedef struct PinStore {
    PinnedHost hosts[MAX_PINNED_HOSTS];
    int count;
} PinStore;

void pinstore_init(PinStore *ps);
int  pinstore_check(const PinStore *ps, const char *hostname, int port,
                    const char *fingerprint);
int  pinstore_add(PinStore *ps, const char *hostname, int port,
                  const char *fingerprint, const char *algo);
int  pinstore_remove(PinStore *ps, const char *hostname, int port);
int  pinstore_load(PinStore *ps, const char *path);
int  pinstore_save(const PinStore *ps, const char *path);

#endif /* PUTTYALT_PINNING_H */
