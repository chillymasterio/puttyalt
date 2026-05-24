#include "puttyalt_sshkeymgr.h"
#include <string.h>
#include <stdio.h>
#include <time.h>

void sshkeymgr_init(SSHKeyManager *km)
{
    memset(km, 0, sizeof(*km));
    km->auto_add_to_agent = 1;
}

int sshkeymgr_add(SSHKeyManager *km, const char *path)
{
    if (km->count >= KEY_MAX || !path) return -1;
    SSHKeyInfo *k = &km->keys[km->count];
    memset(k, 0, sizeof(*k));
    snprintf(k->path, sizeof(k->path), "%s", path);
    k->added_time = (unsigned long)time(NULL);
    
    /* detect type from filename */
    if (strstr(path, "ed25519")) { k->type = KEY_TYPE_ED25519; k->bits = 256; }
    else if (strstr(path, "ecdsa")) { k->type = KEY_TYPE_ECDSA; k->bits = 384; }
    else { k->type = KEY_TYPE_RSA; k->bits = 4096; }
    
    km->count++;
    return km->count - 1;
}

int sshkeymgr_scan(SSHKeyManager *km, const char *directory)
{
    (void)directory;
    /* would scan ~/.ssh/ on the platform */
    return 0;
}

int sshkeymgr_remove(SSHKeyManager *km, int index)
{
    if (index < 0 || index >= km->count) return -1;
    memmove(&km->keys[index], &km->keys[index+1], (km->count - index - 1) * sizeof(SSHKeyInfo));
    km->count--;
    return 0;
}

SSHKeyInfo *sshkeymgr_find_for_host(SSHKeyManager *km, const char *host)
{
    /* look for key with host in comment or matching default */
    for (int i = 0; i < km->count; i++) {
        if (strstr(km->keys[i].comment, host)) return &km->keys[i];
    }
    /* return default if set */
    if (km->default_key_path[0]) {
        for (int i = 0; i < km->count; i++) {
            if (strcmp(km->keys[i].path, km->default_key_path) == 0) return &km->keys[i];
        }
    }
    return km->count > 0 ? &km->keys[0] : NULL;
}

int sshkeymgr_set_default(SSHKeyManager *km, const char *path)
{
    snprintf(km->default_key_path, sizeof(km->default_key_path), "%s", path);
    return 0;
}

const char *sshkeymgr_type_name(int type)
{
    switch (type) {
        case KEY_TYPE_RSA: return "RSA";
        case KEY_TYPE_ED25519: return "Ed25519";
        case KEY_TYPE_ECDSA: return "ECDSA";
        default: return "Unknown";
    }
}
