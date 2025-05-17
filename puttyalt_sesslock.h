/*
 * puttyalt_sesslock.h: Session lock — password-protect active sessions.
 */

#ifndef PUTTYALT_SESSLOCK_H
#define PUTTYALT_SESSLOCK_H

#define SESSLOCK_HASH_LEN    32
#define SESSLOCK_SALT_LEN    16
#define SESSLOCK_MAX_ATTEMPTS 5
#define SESSLOCK_LOCKOUT_SEC  60

typedef struct SessionLock {
    unsigned char password_hash[SESSLOCK_HASH_LEN];
    unsigned char salt[SESSLOCK_SALT_LEN];
    int is_locked;
    int failed_attempts;
    unsigned long lockout_until;   /* epoch seconds */
    int auto_lock_minutes;         /* 0 = disabled */
    unsigned long last_activity;
} SessionLock;

void sesslock_init(SessionLock *lock);
int  sesslock_set_password(SessionLock *lock, const char *password);
int  sesslock_verify(SessionLock *lock, const char *password,
                     unsigned long now);
void sesslock_lock(SessionLock *lock);
int  sesslock_unlock(SessionLock *lock, const char *password,
                     unsigned long now);
int  sesslock_check_autolock(SessionLock *lock, unsigned long now);
void sesslock_activity(SessionLock *lock, unsigned long now);

#endif /* PUTTYALT_SESSLOCK_H */
