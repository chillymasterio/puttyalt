#ifndef PUTTYALT_PASSGEN_H
#define PUTTYALT_PASSGEN_H

typedef struct {
    int length;
    int use_upper;
    int use_lower;
    int use_digits;
    int use_symbols;
    int avoid_ambiguous; /* avoid 0/O, l/1/I */
    int min_entropy_bits;
    char exclude_chars[64];
} PassGenConfig;

void passgen_default(PassGenConfig *cfg);
int  passgen_generate(PassGenConfig *cfg, char *buf, int buflen);
int  passgen_strength(const char *password); /* 0-100 score */
double passgen_entropy(PassGenConfig *cfg);

#endif
