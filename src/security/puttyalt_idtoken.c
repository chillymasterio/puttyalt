/* puttyalt_idtoken.c - OIDC/SSO identity token claims parser (flat JWT payload). */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
typedef struct { char subject[64]; char issuer[96]; char email[96]; long exp; long iat; int valid; } IdToken;
int idtoken_parse_claims(const char *json, IdToken *out) {
    if(!json||!out) return -1; memset(out,0,sizeof(*out));
    const char *p;
    if ((p=strstr(json,"\"sub\""))) { p=strchr(p,':'); if(p){ p=strchr(p,'"'); if(p){ p++; int i=0; while(*p&&*p!='"'&&i<63) out->subject[i++]=*p++; } } }
    if ((p=strstr(json,"\"iss\""))) { p=strchr(p,':'); if(p){ p=strchr(p,'"'); if(p){ p++; int i=0; while(*p&&*p!='"'&&i<95) out->issuer[i++]=*p++; } } }
    if ((p=strstr(json,"\"email\""))) { p=strchr(p,':'); if(p){ p=strchr(p,'"'); if(p){ p++; int i=0; while(*p&&*p!='"'&&i<95) out->email[i++]=*p++; } } }
    if ((p=strstr(json,"\"exp\""))) { p=strchr(p,':'); if(p) out->exp=atol(p+1); }
    if ((p=strstr(json,"\"iat\""))) { p=strchr(p,':'); if(p) out->iat=atol(p+1); }
    out->valid = out->subject[0] && out->issuer[0];
    return out->valid?0:-1;
}
int idtoken_is_expired(const IdToken *t, long now_epoch) {
    if(!t) return 1; return (t->exp>0 && now_epoch>=t->exp)?1:0;
}
int idtoken_summary(const IdToken *t, char *buf, int buflen) {
    if(!t||!buf) return -1;
    return snprintf(buf,buflen,"%s @ %s (exp=%ld)",t->email[0]?t->email:t->subject,t->issuer,t->exp);
}
