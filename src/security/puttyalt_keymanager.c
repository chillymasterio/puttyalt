/*
 * puttyalt_keymanager.c: SSH key inventory implementation.
 * Uses standard C and POSIX APIs only.
 */

#include "puttyalt_keymanager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef _WIN32
#include <dirent.h>
#include <sys/stat.h>
#endif

void keymgr_init(KeyManager *km)
{
    memset(km, 0, sizeof(*km));
}

int keymgr_add(KeyManager *km, const char *path)
{
    if (km->count >= MAX_SSH_KEYS || !path)
        return -1;

    /* Avoid duplicates */
    for (int i = 0; i < km->count; i++)
        if (strcmp(km->keys[i].path, path) == 0)
            return i;

    SSHKeyEntry *k = &km->keys[km->count];
    memset(k, 0, sizeof(*k));
    snprintf(k->path, MAX_KEY_PATH, "%s", path);
    k->format = keymgr_detect_format(path);
    k->has_passphrase = -1;

    /* Guess type from filename */
    if (strstr(path, "ed25519"))
        k->type = KEY_TYPE_ED25519;
    else if (strstr(path, "ecdsa"))
        k->type = KEY_TYPE_ECDSA;
    else if (strstr(path, "dsa"))
        k->type = KEY_TYPE_DSA;
    else if (strstr(path, "rsa"))
        k->type = KEY_TYPE_RSA;
    else
        k->type = KEY_TYPE_UNKNOWN;

    return km->count++;
}

int keymgr_remove(KeyManager *km, int index)
{
    if (index < 0 || index >= km->count)
        return -1;
    for (int i = index; i < km->count - 1; i++)
        km->keys[i] = km->keys[i + 1];
    km->count--;
    return 0;
}

const SSHKeyEntry *keymgr_get(const KeyManager *km, int index)
{
    if (index < 0 || index >= km->count) return NULL;
    return &km->keys[index];
}

int keymgr_search(const KeyManager *km, const char *query,
                  const SSHKeyEntry **results, int max_results)
{
    int n = 0;
    for (int i = 0; i < km->count && n < max_results; i++) {
        if (strstr(km->keys[i].path, query) ||
            strstr(km->keys[i].comment, query))
            results[n++] = &km->keys[i];
    }
    return n;
}

KeyFormat keymgr_detect_format(const char *path)
{
    FILE *f = fopen(path, "r");
    char header[64];
    if (!f) return KEY_FORMAT_UNKNOWN;
    if (!fgets(header, sizeof(header), f)) {
        fclose(f);
        return KEY_FORMAT_UNKNOWN;
    }
    fclose(f);

    if (strstr(header, "PuTTY-User-Key-File"))
        return KEY_FORMAT_PPK;
    if (strstr(header, "OPENSSH PRIVATE KEY") ||
        strstr(header, "-----BEGIN"))
        return KEY_FORMAT_OPENSSH;
    return KEY_FORMAT_UNKNOWN;
}

static int is_key_filename(const char *name)
{
    size_t nlen = strlen(name);
    if (nlen > 4 && strcmp(name + nlen - 4, ".ppk") == 0)
        return 1;
    if (strstr(name, "id_rsa") || strstr(name, "id_ed25519") ||
        strstr(name, "id_ecdsa") || strstr(name, "id_dsa"))
        return 1;
    return 0;
}

#ifndef _WIN32
int keymgr_scan_dir(KeyManager *km, const char *dir)
{
    DIR *d = opendir(dir);
    struct dirent *ent;
    int found = 0;

    if (!d) return 0;

    while ((ent = readdir(d)) != NULL) {
        if (ent->d_name[0] == '.') continue;

        char fullpath[MAX_KEY_PATH];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", dir, ent->d_name);

        struct stat st;
        if (stat(fullpath, &st) != 0 || !S_ISREG(st.st_mode))
            continue;

        if (is_key_filename(ent->d_name) && keymgr_add(km, fullpath) >= 0)
            found++;
    }

    closedir(d);
    return found;
}
#else
int keymgr_scan_dir(KeyManager *km, const char *dir)
{
    /* On Windows, scan known key filenames directly. */
    static const char *key_names[] = {
        "id_rsa", "id_ed25519", "id_ecdsa", "id_dsa",
        "id_rsa.ppk", "id_ed25519.ppk", "id_ecdsa.ppk",
        NULL
    };
    int found = 0;
    for (int i = 0; key_names[i]; i++) {
        char fullpath[MAX_KEY_PATH];
        snprintf(fullpath, sizeof(fullpath), "%s\\%s", dir, key_names[i]);
        FILE *f = fopen(fullpath, "r");
        if (f) {
            fclose(f);
            if (keymgr_add(km, fullpath) >= 0)
                found++;
        }
    }
    return found;
}
#endif

int keymgr_scan_defaults(KeyManager *km)
{
    int found = 0;
    const char *home = getenv(
#ifdef _WIN32
        "USERPROFILE"
#else
        "HOME"
#endif
    );
    if (home) {
        char sshdir[MAX_KEY_PATH];
        snprintf(sshdir, sizeof(sshdir), "%s"
#ifdef _WIN32
            "\\.ssh"
#else
            "/.ssh"
#endif
            , home);
        found += keymgr_scan_dir(km, sshdir);
    }
    return found;
}
