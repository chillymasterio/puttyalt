/*
 * puttyalt_keymanager.c: SSH key inventory implementation.
 */

#include "puttyalt_keymanager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#include <shlobj.h>
#else
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

void keymgr_init(KeyManager *km)
{
    memset(km, 0, sizeof(*km));
}

KeyFormat keymgr_detect_format(const char *path)
{
    FILE *f = fopen(path, "r");
    char buf[64];

    if (!f)
        return KEY_FORMAT_UNKNOWN;

    if (fgets(buf, sizeof(buf), f)) {
        fclose(f);
        if (strncmp(buf, "PuTTY-User-Key-File", 19) == 0)
            return KEY_FORMAT_PPK;
        if (strstr(buf, "PRIVATE KEY"))
            return KEY_FORMAT_OPENSSH;
    } else {
        fclose(f);
    }

    return KEY_FORMAT_UNKNOWN;
}

/*
 * Try to determine key type from the file header.
 */
static KeyType detect_key_type(const char *path)
{
    FILE *f = fopen(path, "r");
    char buf[256];

    if (!f)
        return KEY_TYPE_UNKNOWN;

    while (fgets(buf, sizeof(buf), f)) {
        if (strstr(buf, "ssh-rsa") || strstr(buf, "RSA")) {
            fclose(f);
            return KEY_TYPE_RSA;
        }
        if (strstr(buf, "ssh-ed25519") || strstr(buf, "ED25519")) {
            fclose(f);
            return KEY_TYPE_ED25519;
        }
        if (strstr(buf, "ecdsa") || strstr(buf, "ECDSA")) {
            fclose(f);
            return KEY_TYPE_ECDSA;
        }
        if (strstr(buf, "ssh-dss") || strstr(buf, "DSA")) {
            fclose(f);
            return KEY_TYPE_DSA;
        }
        /* Only check first few lines */
        if (buf[0] != '-' && buf[0] != 'P')
            break;
    }

    fclose(f);
    return KEY_TYPE_UNKNOWN;
}

/*
 * Check if a file looks like an encrypted private key.
 */
static int detect_passphrase(const char *path)
{
    FILE *f = fopen(path, "r");
    char buf[256];

    if (!f)
        return -1;

    while (fgets(buf, sizeof(buf), f)) {
        if (strstr(buf, "ENCRYPTED") || strstr(buf, "Encryption: aes")) {
            fclose(f);
            return 1;
        }
        if (strstr(buf, "Encryption: none")) {
            fclose(f);
            return 0;
        }
    }

    fclose(f);
    return -1;
}

int keymgr_add(KeyManager *km, const char *path)
{
    if (km->count >= MAX_SSH_KEYS)
        return -1;

    /* Reject duplicates */
    for (int i = 0; i < km->count; i++) {
        if (strcmp(km->keys[i].path, path) == 0)
            return -1;
    }

    KeyFormat fmt = keymgr_detect_format(path);
    if (fmt == KEY_FORMAT_UNKNOWN)
        return -1;

    SSHKeyEntry *e = &km->keys[km->count];
    snprintf(e->path, sizeof(e->path), "%s", path);
    e->format = fmt;
    e->type = detect_key_type(path);
    e->has_passphrase = detect_passphrase(path);
    e->bits = 0;   /* TODO: parse actual key bits */
    e->comment[0] = '\0';

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
    if (index < 0 || index >= km->count)
        return NULL;
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

#ifdef _WIN32
int keymgr_scan_dir(KeyManager *km, const char *dir)
{
    char pattern[MAX_KEY_PATH];
    WIN32_FIND_DATAA fd;
    int found = 0;

    snprintf(pattern, sizeof(pattern), "%s\\*", dir);
    HANDLE h = FindFirstFileA(pattern, &fd);
    if (h == INVALID_HANDLE_VALUE)
        return 0;

    do {
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            continue;

        char fullpath[MAX_KEY_PATH];
        snprintf(fullpath, sizeof(fullpath), "%s\\%s", dir, fd.cFileName);

        /* Check for key-like extensions or names */
        const char *name = fd.cFileName;
        size_t nlen = strlen(name);
        int is_key = 0;

        if (nlen > 4 && strcmp(name + nlen - 4, ".ppk") == 0)
            is_key = 1;
        else if (strstr(name, "id_rsa") || strstr(name, "id_ed25519") ||
                 strstr(name, "id_ecdsa") || strstr(name, "id_dsa"))
            is_key = 1;

        if (is_key && keymgr_add(km, fullpath) >= 0)
            found++;
    } while (FindNextFileA(h, &fd));

    FindClose(h);
    return found;
}
#else
int keymgr_scan_dir(KeyManager *km, const char *dir)
{
    DIR *d = opendir(dir);
    struct dirent *ent;
    int found = 0;

    if (!d)
        return 0;

    while ((ent = readdir(d)) != NULL) {
        if (ent->d_name[0] == '.')
            continue;

        char fullpath[MAX_KEY_PATH];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", dir, ent->d_name);

        struct stat st;
        if (stat(fullpath, &st) != 0 || !S_ISREG(st.st_mode))
            continue;

        const char *name = ent->d_name;
        size_t nlen = strlen(name);
        int is_key = 0;

        if (nlen > 4 && strcmp(name + nlen - 4, ".ppk") == 0)
            is_key = 1;
        else if (strstr(name, "id_rsa") || strstr(name, "id_ed25519") ||
                 strstr(name, "id_ecdsa") || strstr(name, "id_dsa"))
            is_key = 1;

        if (is_key && keymgr_add(km, fullpath) >= 0)
            found++;
    }

    closedir(d);
    return found;
}
#endif

int keymgr_scan_defaults(KeyManager *km)
{
    int found = 0;

#ifdef _WIN32
    char home[MAX_KEY_PATH];
    if (GetEnvironmentVariableA("USERPROFILE", home, sizeof(home))) {
        char sshdir[MAX_KEY_PATH];
        snprintf(sshdir, sizeof(sshdir), "%s\\.ssh", home);
        found += keymgr_scan_dir(km, sshdir);
    }
#else
    const char *home = getenv("HOME");
    if (home) {
        char sshdir[MAX_KEY_PATH];
        snprintf(sshdir, sizeof(sshdir), "%s/.ssh", home);
        found += keymgr_scan_dir(km, sshdir);
    }
#endif

    return found;
}
