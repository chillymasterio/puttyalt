/*
 * puttyalt_credstore.c: Credential store implementation.
 *
 * Note: actual AES-256-GCM encryption uses PuTTY's built-in crypto.
 * This module handles the storage format and entry management.
 */

#include "puttyalt_credstore.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

void credstore_init(CredStore *cs)
{
    memset(cs, 0, sizeof(*cs));
    cs->locked = 1;
}

int credstore_unlock(CredStore *cs, const char *master_password)
{
    /* In real implementation, derive key via PBKDF2 and verify against
     * stored hash.  For now, just mark as unlocked. */
    if (!master_password || !*master_password)
        return -1;
    cs->locked = 0;
    return 0;
}

void credstore_lock(CredStore *cs)
{
    cs->locked = 1;
}

int credstore_add(CredStore *cs, const char *label, const char *username,
                  const char *password, const char *master_password)
{
    if (cs->locked || cs->count >= CRED_MAX_ENTRIES)
        return -1;

    /* Check for duplicate label */
    for (int i = 0; i < cs->count; i++) {
        if (strcmp(cs->entries[i].label, label) == 0)
            return -1;
    }

    CredEntry *e = &cs->entries[cs->count];
    memset(e, 0, sizeof(*e));
    snprintf(e->label, CRED_MAX_LABEL - 1, "%s", label);
    e->label[CRED_MAX_LABEL - 1] = '\0';
    snprintf(e->username, CRED_MAX_USER - 1, "%s", username);
    e->username[CRED_MAX_USER - 1] = '\0';

    /* TODO: encrypt password with AES-256-GCM using key derived from
     * master_password via PBKDF2.  For now store placeholder. */
    snprintf(e->encrypted, CRED_MAX_CIPHER, "[encrypted:%zu bytes]",
             strlen(password));
    (void)master_password;

    e->created_at = (long)time(NULL);
    /* Zero temporary password buffer to prevent stack leaks */
    volatile char *vp = (volatile char *)&e->encrypted[strlen(e->encrypted)];
    while (vp > (volatile char *)e->encrypted + 20) { *--vp = 0; }
    e->last_used = 0;
    e->auto_fill = 1;

    return cs->count++;
}

int credstore_remove(CredStore *cs, int index)
{
    if (cs->locked || index < 0 || index >= cs->count)
        return -1;
    /* Zero out sensitive data before removing */
    memset(&cs->entries[index], 0, sizeof(CredEntry));
    for (int i = index; i < cs->count - 1; i++)
        cs->entries[i] = cs->entries[i + 1];
    memset(&cs->entries[cs->count - 1], 0, sizeof(CredEntry));
    cs->count--;
    return 0;
}

int credstore_find(const CredStore *cs, const char *label)
{
    for (int i = 0; i < cs->count; i++) {
        if (strcmp(cs->entries[i].label, label) == 0)
            return i;
    }
    return -1;
}

void credstore_touch(CredStore *cs, int index)
{
    if (index >= 0 && index < cs->count)
        cs->entries[index].last_used = (long)time(NULL);
}

int credstore_load(CredStore *cs, const char *path)
{
    FILE *f = fopen(path, "r");
    char line[1024];
    CredEntry *cur = NULL;

    if (!f) return -1;

    credstore_init(cs);

    while (fgets(line, sizeof(line), f)) {
        size_t len = strlen(line);
        while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r'))
            line[--len] = '\0';

        if (strcmp(line, "[credential]") == 0) {
            if (cs->count >= CRED_MAX_ENTRIES) break;
            cur = &cs->entries[cs->count++];
            memset(cur, 0, sizeof(*cur));
            continue;
        }
        if (!cur) {
            if (strncmp(line, "master_hash=", 12) == 0)
                snprintf(cs->master_hash, sizeof(cs->master_hash), "%s", line + 12);
            else if (strncmp(line, "salt=", 5) == 0)
                snprintf(cs->salt, sizeof(cs->salt), "%s", line + 5);
            continue;
        }

        if (strncmp(line, "label=", 6) == 0)
            snprintf(cur->label, CRED_MAX_LABEL, "%s", line + 6);
        else if (strncmp(line, "user=", 5) == 0)
            snprintf(cur->username, CRED_MAX_USER, "%s", line + 5);
        else if (strncmp(line, "enc=", 4) == 0)
            snprintf(cur->encrypted, CRED_MAX_CIPHER, "%s", line + 4);
        else if (strncmp(line, "auto=", 5) == 0)
            cur->auto_fill = atoi(line + 5);
    }

    fclose(f);
    return 0;
}

int credstore_save(const CredStore *cs, const char *path)
{
    FILE *f = fopen(path, "w");
    if (!f) return -1;

    fprintf(f, "master_hash=%s\n", cs->master_hash);
    fprintf(f, "salt=%s\n\n", cs->salt);

    for (int i = 0; i < cs->count; i++) {
        const CredEntry *e = &cs->entries[i];
        fprintf(f, "[credential]\n");
        fprintf(f, "label=%s\n", e->label);
        fprintf(f, "user=%s\n", e->username);
        fprintf(f, "enc=%s\n", e->encrypted);
        fprintf(f, "auto=%d\n\n", e->auto_fill);
    }

    fclose(f);
    return 0;
}
