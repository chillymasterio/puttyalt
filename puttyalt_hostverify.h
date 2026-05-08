#ifndef PUTTYALT_HOSTVERIFY_H
#define PUTTYALT_HOSTVERIFY_H

#define HV_MAX_KNOWN   256
#define HV_FP_LEN      128

typedef enum {
    HV_UNKNOWN = 0,
    HV_TRUSTED,
    HV_CHANGED,
    HV_REVOKED
} HostVerifyStatus;

typedef struct KnownHost {
    char hostname[256];
    int port;
    char key_type[32];
    char fingerprint[HV_FP_LEN];
    unsigned long first_seen;
    unsigned long last_seen;
    HostVerifyStatus status;
} KnownHost;

typedef struct KnownHostDB {
    KnownHost hosts[HV_MAX_KNOWN];
    int count;
    char db_path[512];
} KnownHostDB;

int  hv_load(KnownHostDB *db, const char *path);
int  hv_save(const KnownHostDB *db);
HostVerifyStatus hv_check(const KnownHostDB *db, const char *host, int port,
                          const char *fingerprint);
int  hv_add(KnownHostDB *db, const char *host, int port,
            const char *key_type, const char *fingerprint, unsigned long now);
int  hv_remove(KnownHostDB *db, const char *host, int port);
int  hv_revoke(KnownHostDB *db, const char *host, int port);

#endif
