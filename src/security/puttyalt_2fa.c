#include <string.h>
#include <stdio.h>
#include <time.h>
typedef struct { int enabled; char secret[64]; int digits; int period; } TwoFactorConfig;
static TwoFactorConfig g_2fa = {0, "", 6, 30};
void twofa_configure(const char *secret, int digits, int period) {
    snprintf(g_2fa.secret, 64, "%s", secret); g_2fa.digits = digits; g_2fa.period = period; g_2fa.enabled = 1;
}
int twofa_is_enabled(void) { return g_2fa.enabled; }
int twofa_time_remaining(void) {
    time_t now = time(NULL); return g_2fa.period - (int)(now % g_2fa.period);
}
int twofa_prompt_needed(const char *server_msg) {
    return strstr(server_msg, "Verification code") || strstr(server_msg, "2FA") || strstr(server_msg, "OTP") || strstr(server_msg, "one-time");
}
void twofa_disable(void) { g_2fa.enabled = 0; memset(g_2fa.secret, 0, 64); }
