/* puttyalt_pwstrength.c - Passphrase strength estimator (entropy bits + class checks). */
#include <string.h>
#include <stdio.h>
enum pw_verdict { PW_VERY_WEAK=0, PW_WEAK=1, PW_FAIR=2, PW_STRONG=3, PW_VERY_STRONG=4 };
typedef struct { int has_lower, has_upper, has_digit, has_symbol; int len; int entropy_bits; } PwStrength;
int pwstrength_eval(const char *pw, PwStrength *out) {
    if(!pw||!out) return -1;
    memset(out,0,sizeof(*out));
    out->len=(int)strlen(pw);
    for (const char *p=pw; *p; p++) {
        if (*p>='a'&&*p<='z') out->has_lower=1;
        else if (*p>='A'&&*p<='Z') out->has_upper=1;
        else if (*p>='0'&&*p<='9') out->has_digit=1;
        else out->has_symbol=1;
    }
    int pool=0;
    if (out->has_lower) pool+=26;
    if (out->has_upper) pool+=26;
    if (out->has_digit) pool+=10;
    if (out->has_symbol) pool+=33;
    /* entropy = len * log2(pool); approximate log2 via integer table */
    int log2x10=0, p=pool;
    while (p>1) { p>>=1; log2x10+=10; } /* floor(log2)*10 */
    out->entropy_bits=(out->len*log2x10)/10;
    return 0;
}
int pwstrength_verdict(const PwStrength *s) {
    if(!s) return -1;
    int e=s->entropy_bits;
    if (e<28) return PW_VERY_WEAK;
    if (e<36) return PW_WEAK;
    if (e<60) return PW_FAIR;
    if (e<128) return PW_STRONG;
    return PW_VERY_STRONG;
}
int pwstrength_describe(const PwStrength *s, char *buf, int buflen) {
    if(!s||!buf) return -1;
    static const char *names[]={"very weak","weak","fair","strong","very strong"};
    return snprintf(buf,buflen,"%d bits, %s (len=%d classes=%d)",
        s->entropy_bits, names[pwstrength_verdict(s)], s->len,
        s->has_lower+s->has_upper+s->has_digit+s->has_symbol);
}
