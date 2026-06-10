/* puttyalt_locale2.c - Locale-aware number/date formatting. */
#include <string.h>
#include <stdio.h>
typedef struct { char decimal_sep; char thousands_sep; int date_order; char currency[8]; } Locale2;
void locale2_init(Locale2 *l, const char *locale) {
    if(!l) return; memset(l,0,sizeof(*l));
    if (locale && (!strncmp(locale,"de",2)||!strncmp(locale,"fr",2)||!strncmp(locale,"ru",2))) {
        l->decimal_sep=','; l->thousands_sep='.'; l->date_order=1; /* DMY */
    } else if (locale && !strncmp(locale,"en_US",5)) {
        l->decimal_sep='.'; l->thousands_sep=','; l->date_order=2; /* MDY */
    } else { l->decimal_sep='.'; l->thousands_sep=','; l->date_order=0; /* YMD */ }
    snprintf(l->currency,8,"%s","$");
}
int locale2_format_number(const Locale2 *l, long n, char *out, int outlen) {
    if(!l||!out) return -1;
    char tmp[32]; int len=snprintf(tmp,sizeof(tmp),"%ld",n<0?-n:n);
    int o=0; if(n<0&&o<outlen-1) out[o++]='-';
    for (int i=0;i<len && o<outlen-1;i++) {
        if (i>0 && (len-i)%3==0) out[o++]=l->thousands_sep;
        out[o++]=tmp[i];
    }
    out[o]=0; return o;
}
int locale2_format_date(const Locale2 *l, int y, int m, int d, char *out, int outlen) {
    if(!l||!out) return -1;
    switch(l->date_order) {
        case 1: return snprintf(out,outlen,"%02d.%02d.%04d",d,m,y); /* DMY */
        case 2: return snprintf(out,outlen,"%02d/%02d/%04d",m,d,y); /* MDY */
        default: return snprintf(out,outlen,"%04d-%02d-%02d",y,m,d); /* YMD ISO */
    }
}
