/* puttyalt_certvalid.c - Validate certificate date windows.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { long not_before; long not_after; } CertWindow;
/* Is the certificate valid at `now`? */
int cv2_is_valid(const CertWindow *c, long now) {
    if (!c) return 0;
    return now >= c->not_before && now <= c->not_after;
}
int cv2_is_expired(const CertWindow *c, long now) { return c && now > c->not_after; }
int cv2_not_yet_valid(const CertWindow *c, long now) { return c && now < c->not_before; }
long cv2_days_remaining(const CertWindow *c, long now) {
    if (!c) return 0;
    long secs = c->not_after - now;
    return secs > 0 ? secs / 86400 : 0;
}
int cv2_should_warn(const CertWindow *c, long now, int warn_days) {
    return cv2_is_valid(c, now) && cv2_days_remaining(c, now) <= warn_days;
}
