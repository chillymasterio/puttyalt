#ifndef PUTTYALT_FINGERPRINT_H
#define PUTTYALT_FINGERPRINT_H

#define FP_MAX_HOSTS    256
#define FP_MAX_HOST     256
#define FP_MAX_FP       128
#define FP_MAX_ALGO     32

typedef enum {
    FP_TRUST_UNKNOWN = 0,
    FP_TRUST_TRUSTED,
    FP_TRUST_REVOKED,
    FP_TRUST_CHANGED
} FPTrustLevel;

typedef struct {
    char          hostname[FP_MAX_HOST];
    int           port;
    char          fingerprint[FP_MAX_FP];
    char          algo[FP_MAX_ALGO];
    FPTrustLevel  trust;
    long          first_seen;
    long          last_seen;
    int           seen_count;
} FPEntry;

typedef struct {
    FPEntry entries[FP_MAX_HOSTS];
    int     count;
} FPStore;

void fpstore_init(FPStore *fps);
int  fpstore_add(FPStore *fps, const char *host, int port,
                 const char *fingerprint, const char *algo);
int  fpstore_find(const FPStore *fps, const char *host, int port);
FPTrustLevel fpstore_verify(FPStore *fps, const char *host, int port,
                            const char *fingerprint);
int  fpstore_trust(FPStore *fps, int index);
int  fpstore_revoke(FPStore *fps, int index);
int  fpstore_remove(FPStore *fps, int index);
int  fpstore_load(FPStore *fps, const char *path);
int  fpstore_save(const FPStore *fps, const char *path);
int  fpstore_export_known_hosts(const FPStore *fps, const char *path);

#endif
