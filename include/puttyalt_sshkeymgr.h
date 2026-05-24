#ifndef PUTTYALT_SSHKEYMGR_H
#define PUTTYALT_SSHKEYMGR_H

#define KEY_MAX 32
#define KEY_TYPE_RSA      0
#define KEY_TYPE_ED25519  1
#define KEY_TYPE_ECDSA    2

typedef struct {
    char path[512];
    char fingerprint[128];
    char comment[128];
    int  type;
    int  bits;
    int  has_passphrase;
    int  loaded_in_agent;
    unsigned long added_time;
} SSHKeyInfo;

typedef struct {
    SSHKeyInfo keys[KEY_MAX];
    int count;
    char default_key_path[512];
    int  auto_add_to_agent;
} SSHKeyManager;

void sshkeymgr_init(SSHKeyManager *km);
int  sshkeymgr_scan(SSHKeyManager *km, const char *directory);
int  sshkeymgr_add(SSHKeyManager *km, const char *path);
int  sshkeymgr_remove(SSHKeyManager *km, int index);
SSHKeyInfo *sshkeymgr_find_for_host(SSHKeyManager *km, const char *host);
int  sshkeymgr_set_default(SSHKeyManager *km, const char *path);
const char *sshkeymgr_type_name(int type);

#endif
