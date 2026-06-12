/* puttyalt_secretref.c - Secret reference resolver (env/vault/file indirection). */
#include <string.h>
#include <stdio.h>
enum sr_source { SR_LITERAL=0, SR_ENV=1, SR_VAULT=2, SR_FILE=3, SR_KEYCHAIN=4 };
typedef struct { int source; char ref[160]; } SecretRef;
int secretref_parse(const char *str, SecretRef *out) {
    if(!str||!out) return -1;
    memset(out,0,sizeof(*out));
    if (!strncmp(str,"env:",4)) { out->source=SR_ENV; snprintf(out->ref,160,"%s",str+4); }
    else if (!strncmp(str,"vault:",6)) { out->source=SR_VAULT; snprintf(out->ref,160,"%s",str+6); }
    else if (!strncmp(str,"file:",5)) { out->source=SR_FILE; snprintf(out->ref,160,"%s",str+5); }
    else if (!strncmp(str,"keychain:",9)) { out->source=SR_KEYCHAIN; snprintf(out->ref,160,"%s",str+9); }
    else { out->source=SR_LITERAL; snprintf(out->ref,160,"%s",str); }
    return 0;
}
int secretref_is_indirect(const SecretRef *s) { return (s&&s->source!=SR_LITERAL)?1:0; }
const char *secretref_source_name(int source) {
    static const char *n[]={"literal","env","vault","file","keychain"};
    return (source>=0&&source<=SR_KEYCHAIN)?n[source]:"?";
}
int secretref_format(const SecretRef *s, char *buf, int buflen) {
    if(!s||!buf) return -1;
    if (s->source==SR_LITERAL) return snprintf(buf,buflen,"***");
    return snprintf(buf,buflen,"%s:%s",secretref_source_name(s->source),s->ref);
}
