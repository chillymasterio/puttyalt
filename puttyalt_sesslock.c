/*
 * puttyalt_sesslock.c: Session lock implementation.
 *
 * Uses a simple PBKDF2-like hash for demonstration.
 * In production, links to the crypto library's proper KDF.
 */

#include <string.h>
#include <stdlib.h>
#include "puttyalt_sesslock.h"

/* Simple hash — in real build this calls crypto/hash */
static void compute_hash(const char *password, const unsigned char *salt,
                         unsigned char *out)
{
    unsigned int h = 5381;
    for (int i = 0; i < SESSLOCK_SALT_LEN; i++)
        h = h * 33 + salt[i];
    while (*password)
        h = h * 33 + (unsigned char)*password++;

    /* Stretch into SESSLOCK_HASH_LEN bytes */
    for (int i = 0; i < SESSLOCK_HASH_LEN; i++) {
        out[i] = (unsigned char)(h & 0xFF);
        h = h * 2654435761u + i;
    }
}

void sesslock_init(SessionLock *lock)
{
    memset(lock, 0, sizeof(*lock));
    lock->auto_lock_minutes = 0;
}

int sesslock_set_password(SessionLock *lock, const char *password)
{
    if (!password || strlen(password) < 4)
        return -1;

    /* Generate pseudo-random salt */
    for (int i = 0; i < SESSLOCK_SALT_LEN; i++)
        lock->salt[i] = (unsigned char)(rand() & 0xFF);

    compute_hash(password, lock->salt, lock->password_hash);
    return 0;
}

int sesslock_verify(SessionLock *lock, const char *password, unsigned long now)
{
    if (lock->lockout_until > now)
        return -2; /* locked out */

    unsigned char test[SESSLOCK_HASH_LEN];
    compute_hash(password, lock->salt, test);

    /* Constant-time comparison to prevent timing attacks */
    int diff = 0;
    for (int i = 0; i < SESSLOCK_HASH_LEN; i++)
        diff |= test[i] ^ lock->password_hash[i];
    if (diff != 0) {
        lock->failed_attempts++;
        if (lock->failed_attempts >= SESSLOCK_MAX_ATTEMPTS)
            lock->lockout_until = now + SESSLOCK_LOCKOUT_SEC;
        return -1;
    }

    lock->failed_attempts = 0;
    lock->lockout_until = 0;
    return 0;
}

void sesslock_lock(SessionLock *lock)
{
    lock->is_locked = 1;
}

int sesslock_unlock(SessionLock *lock, const char *password, unsigned long now)
{
    if (!lock->is_locked)
        return 0;

    int rc = sesslock_verify(lock, password, now);
    if (rc == 0)
        lock->is_locked = 0;
    return rc;
}

int sesslock_check_autolock(SessionLock *lock, unsigned long now)
{
    if (lock->auto_lock_minutes <= 0)
        return 0;
    if (lock->is_locked)
        return 0;

    unsigned long idle = now - lock->last_activity;
    if (idle >= (unsigned long)lock->auto_lock_minutes * 60) {
        lock->is_locked = 1;
        return 1;
    }
    return 0;
}

void sesslock_activity(SessionLock *lock, unsigned long now)
{
    lock->last_activity = now;
}
