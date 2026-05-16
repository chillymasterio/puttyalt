#include "puttyalt_passgen.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

static const char UPPER[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static const char LOWER[] = "abcdefghijklmnopqrstuvwxyz";
static const char DIGIT[] = "0123456789";
static const char SYMBOL[] = "!@#$%^&*()-_=+[]{}|;:,.<>?";
static const char AMBIGUOUS[] = "0OolI1";

void passgen_default(PassGenConfig *cfg)
{
    memset(cfg, 0, sizeof(*cfg));
    cfg->length = 20;
    cfg->use_upper = 1;
    cfg->use_lower = 1;
    cfg->use_digits = 1;
    cfg->use_symbols = 1;
    cfg->avoid_ambiguous = 1;
    cfg->min_entropy_bits = 80;
}

static int is_excluded(PassGenConfig *cfg, char c)
{
    if (cfg->avoid_ambiguous && strchr(AMBIGUOUS, c)) return 1;
    if (cfg->exclude_chars[0] && strchr(cfg->exclude_chars, c)) return 1;
    return 0;
}

static int build_charset(PassGenConfig *cfg, char *charset)
{
    int n = 0;
    const char *sets[] = { NULL, NULL, NULL, NULL };
    int idx = 0;
    if (cfg->use_upper) sets[idx++] = UPPER;
    if (cfg->use_lower) sets[idx++] = LOWER;
    if (cfg->use_digits) sets[idx++] = DIGIT;
    if (cfg->use_symbols) sets[idx++] = SYMBOL;
    
    for (int s = 0; s < idx; s++) {
        for (const char *p = sets[s]; *p; p++) {
            if (!is_excluded(cfg, *p)) charset[n++] = *p;
        }
    }
    charset[n] = '\0';
    return n;
}

int passgen_generate(PassGenConfig *cfg, char *buf, int buflen)
{
    char charset[256];
    int clen = build_charset(cfg, charset);
    if (clen == 0 || cfg->length <= 0) return -1;
    int len = cfg->length < buflen ? cfg->length : buflen - 1;
    
    /* seed from time + address entropy */
    srand((unsigned)(time(NULL) ^ (unsigned long)(void*)buf));
    
    for (int i = 0; i < len; i++)
        buf[i] = charset[rand() % clen];
    buf[len] = '\0';
    return len;
}

int passgen_strength(const char *password)
{
    if (!password || !password[0]) return 0;
    int len = (int)strlen(password);
    int has_upper = 0, has_lower = 0, has_digit = 0, has_sym = 0;
    for (int i = 0; i < len; i++) {
        char c = password[i];
        if (c >= 'A' && c <= 'Z') has_upper = 1;
        else if (c >= 'a' && c <= 'z') has_lower = 1;
        else if (c >= '0' && c <= '9') has_digit = 1;
        else has_sym = 1;
    }
    int charset_size = has_lower * 26 + has_upper * 26 + has_digit * 10 + has_sym * 30;
    if (charset_size == 0) return 0;
    double entropy = len * log2((double)charset_size);
    if (entropy >= 128) return 100;
    if (entropy >= 80) return 80;
    if (entropy >= 60) return 60;
    if (entropy >= 40) return 40;
    return 20;
}

double passgen_entropy(PassGenConfig *cfg)
{
    char charset[256];
    int clen = build_charset(cfg, charset);
    if (clen <= 1) return 0;
    return cfg->length * log2((double)clen);
}
