#include "puttyalt_passgen.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <math.h>
#ifdef _WIN32
#include <windows.h>
#endif

static const char UPPER[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static const char LOWER[] = "abcdefghijklmnopqrstuvwxyz";
static const char DIGIT[] = "0123456789";
static const char SYMBOL[] = "!@#$%^&*()-_=+[]{}|;:,.<>?";
static const char AMBIGUOUS[] = "0OolI1";

/* Estimate Shannon entropy of a given password string */
static double estimate_shannon_entropy(const char *password, int len)
{
    int freq[256] = {0};
    for (int i = 0; i < len; i++)
        freq[(unsigned char)password[i]]++;

    double entropy = 0.0;
    for (int i = 0; i < 256; i++) {
        if (freq[i] == 0) continue;
        double p = (double)freq[i] / len;
        entropy -= p * log2(p);
    }
    return entropy * len;
}

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
    
    /* Use OS entropy source for cryptographic quality randomness */
#ifdef _WIN32
    /* Use CryptGenRandom via RtlGenRandom (SystemFunction036) */
    {
        unsigned char randbuf[256];
        typedef int (WINAPI *RtlGenRandomFn)(void*, unsigned long);
        HMODULE advapi = LoadLibraryA("advapi32.dll");
        RtlGenRandomFn gen = advapi ?
            (RtlGenRandomFn)GetProcAddress(advapi, "SystemFunction036") : NULL;
        if (gen && gen(randbuf, (unsigned long)len)) {
            for (int i = 0; i < len; i++)
                buf[i] = charset[randbuf[i] % clen];
        } else {
            /* Fallback: mix multiple entropy sources */
            unsigned int seed = (unsigned int)time(NULL);
            seed ^= (unsigned int)(uintptr_t)buf;
            seed ^= (unsigned int)GetTickCount();
            srand(seed);
            for (int i = 0; i < len; i++)
                buf[i] = charset[rand() % clen];
        }
        if (advapi) FreeLibrary(advapi);
    }
#else
    {
        FILE *urand = fopen("/dev/urandom", "rb");
        if (urand) {
            unsigned char randbuf[256];
            int rlen = len < 256 ? len : 256;
            if (fread(randbuf, 1, rlen, urand) == (size_t)rlen) {
                for (int i = 0; i < len; i++)
                    buf[i] = charset[randbuf[i % rlen] % clen];
            }
            fclose(urand);
        } else {
            srand((unsigned)(time(NULL) ^ (unsigned long)(void*)buf));
            for (int i = 0; i < len; i++)
                buf[i] = charset[rand() % clen];
        }
    }
#endif
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

double passgen_actual_entropy(const char *password)
{
    if (!password || !password[0]) return 0.0;
    int len = (int)strlen(password);
    return estimate_shannon_entropy(password, len);
}
