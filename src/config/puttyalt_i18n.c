/* puttyalt_i18n.c - Internationalization string table with locale fallback. */
#include <string.h>
#include <stdio.h>
#define I18N_MAX 256
#define I18N_KEY 48
#define I18N_VAL 160
typedef struct { char key[I18N_KEY]; char value[I18N_VAL]; } i18n_entry;
typedef struct { i18n_entry entries[I18N_MAX]; int n; char locale[16]; } I18n;
void i18n_init(I18n *i, const char *locale) {
    if(!i) return;
    memset(i,0,sizeof(*i)); snprintf(i->locale,16,"%s",locale?locale:"en");
}
int i18n_set(I18n *i, const char *key, const char *value) {
    if(!i||!key) return -1;
    for (int j=0;j<i->n;j++) if (strcmp(i->entries[j].key,key)==0) { snprintf(i->entries[j].value,I18N_VAL,"%s",value?value:""); return 0; }
    if (i->n>=I18N_MAX) return -1;
    snprintf(i->entries[i->n].key,I18N_KEY,"%s",key); snprintf(i->entries[i->n].value,I18N_VAL,"%s",value?value:""); i->n++;
    return 0;
}
const char *i18n_get(const I18n *i, const char *key) {
    if(!i||!key) return key;
    for (int j=0;j<i->n;j++) if (strcmp(i->entries[j].key,key)==0) return i->entries[j].value;
    return key; /* fallback to key */
}
int i18n_format(const I18n *i, const char *key, const char *arg, char *out, int outlen) {
    if(!i||!out) return -1;
    const char *tmpl=i18n_get(i,key);
    const char *pct=strstr(tmpl,"%s");
    if (!pct) return snprintf(out,outlen,"%s",tmpl);
    int pre=(int)(pct-tmpl);
    return snprintf(out,outlen,"%.*s%s%s",pre,tmpl,arg?arg:"",pct+2);
}
int i18n_count(const I18n *i) { return i?i->n:-1; }
