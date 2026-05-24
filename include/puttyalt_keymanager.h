/*
 * puttyalt_keymanager.h: SSH key inventory and quick-select.
 *
 * Scans well-known directories for SSH private keys (.ppk, OpenSSH)
 * and presents a pick-list in the session dialog.  Tracks per-session
 * key assignments so users don't have to browse every time.
 */

#ifndef PUTTYALT_KEYMANAGER_H
#define PUTTYALT_KEYMANAGER_H

#define MAX_SSH_KEYS      128
#define MAX_KEY_PATH      512
#define MAX_KEY_COMMENT    256

typedef enum {
    KEY_FORMAT_PPK,         /* PuTTY .ppk */
    KEY_FORMAT_OPENSSH,     /* OpenSSH private key */
    KEY_FORMAT_UNKNOWN
} KeyFormat;

typedef enum {
    KEY_TYPE_RSA,
    KEY_TYPE_ED25519,
    KEY_TYPE_ECDSA,
    KEY_TYPE_DSA,
    KEY_TYPE_UNKNOWN
} KeyType;

typedef struct SSHKeyEntry {
    char     path[MAX_KEY_PATH];
    char     comment[MAX_KEY_COMMENT];
    KeyFormat format;
    KeyType  type;
    int      bits;
    int      has_passphrase;    /* 1 if encrypted, 0 if not, -1 if unknown */
} SSHKeyEntry;

typedef struct KeyManager {
    SSHKeyEntry keys[MAX_SSH_KEYS];
    int count;
} KeyManager;

/* Initialise key manager */
void keymgr_init(KeyManager *km);

/* Scan a directory for SSH key files.  Returns number of keys found. */
int keymgr_scan_dir(KeyManager *km, const char *dir);

/* Scan default directories (~/.ssh, PuTTY registry, etc.) */
int keymgr_scan_defaults(KeyManager *km);

/* Add a key manually by path */
int keymgr_add(KeyManager *km, const char *path);

/* Remove a key by index */
int keymgr_remove(KeyManager *km, int index);

/* Get a key entry by index */
const SSHKeyEntry *keymgr_get(const KeyManager *km, int index);

/* Find keys matching a substring in path or comment */
int keymgr_search(const KeyManager *km, const char *query,
                  const SSHKeyEntry **results, int max_results);

/* Detect format of a key file */
KeyFormat keymgr_detect_format(const char *path);

#endif /* PUTTYALT_KEYMANAGER_H */
