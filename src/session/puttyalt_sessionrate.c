/* puttyalt_sessionrate.c - Rate session quality from metrics.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { long latency_ms; int packet_loss_pct; long jitter_ms; } SessionMetrics;
/* Score 0-100 from latency, loss, jitter. */
int srt_score(const SessionMetrics *m) {
    if (!m) return 0;
    int score = 100;
    if (m->latency_ms > 50) score -= (int)((m->latency_ms - 50) / 10);
    score -= m->packet_loss_pct * 5;
    score -= (int)(m->jitter_ms / 5);
    if (score < 0) score = 0;
    if (score > 100) score = 100;
    return score;
}
const char *srt_grade(int score) {
    if (score >= 90) return "excellent";
    if (score >= 75) return "good";
    if (score >= 50) return "fair";
    if (score >= 25) return "poor";
    return "bad";
}
