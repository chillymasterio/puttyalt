/*
 * puttyalt_credstore.h: Encrypted credential storage.
 *
 * Stores SSH passwords and passphrases encrypted with AES-256-GCM,
 * protected by a master password.  Credentials are per-session.
 */

#ifndef PUTTYALT_CREDSTORE_H
#define PUTTYALT_CREDSTORE_H

#define CRED_MAX_ENTRIES  128
#define CRED_MAX_LABEL    128
#define CRED_MAX_USER     128
#define CRED_MAX_CIPHER   512   /* base64-encoded encrypted password */

typedef struct CredEntry {
    char label[CRED_MAX_LABEL];
    char username[CRED_MAX_USER];
    char encrypted[CRED_MAX_CIPHER];
    long created_at;
    long last_used;
    int  auto_fill;             /* auto-fill on connect */
} CredEntry;

typedef struct CredStore {
    CredEntry entries[CRED_MAX_ENTRIES];
    int       count;
    int       locked;           /* 1 = master password not entered yet */
    char      master_hash[128]; /* hash of master password for verification */
    char      salt[64];
} CredStore;

void credstore_init(CredStore *cs);
int  credstore_unlock(CredStore *cs, const char *master_password);
void credstore_lock(CredStore *cs);
int  credstore_add(CredStore *cs, const char *label, const char *username,
                   const char *password, const char *master_password);
int  credstore_remove(CredStore *cs, int index);
int  credstore_find(const CredStore *cs, const char *label);
int  credstore_load(CredStore *cs, const char *path);
int  credstore_save(const CredStore *cs, const char *path);
void credstore_touch(CredStore *cs, int index);

#endif /* PUTTYALT_CREDSTORE_H */
