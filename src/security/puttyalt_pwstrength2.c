/* puttyalt_pwstrength2.c - Comprehensive password strength scoring.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <ctype.h>
#include <string.h>
/* Score 0-100 considering length, variety, and common patterns. */
int pws_score(const char *pw) {
    if (!pw) return 0;
    int len = (int)strlen(pw);
    int score = 0;
    int up=0,lo=0,dg=0,sp=0;
    for (const char *p = pw; *p; p++) {
        if (isupper((unsigned char)*p)) up=1;
        else if (islower((unsigned char)*p)) lo=1;
        else if (isdigit((unsigned char)*p)) dg=1;
        else sp=1;
    }
    score += len * 4; if (score > 40) score = 40;
    score += (up+lo+dg+sp) * 10;
    /* penalize sequences */
    int seq = 0;
    for (int i = 2; i < len; i++)
        if (pw[i] == pw[i-1] + 1 && pw[i-1] == pw[i-2] + 1) seq++;
    score -= seq * 5;
    if (score < 0) score = 0;
    if (score > 100) score = 100;
    return score;
}
int pws_has_repeat(const char *pw) {
    if (!pw) return 0;
    for (int i = 2; pw[i]; i++) if (pw[i] == pw[i-1] && pw[i-1] == pw[i-2]) return 1;
    return 0;
}
