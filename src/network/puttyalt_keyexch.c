/* puttyalt_keyexch.c - KEX algorithm preference negotiation (client/server intersection). */
#include <string.h>
#include <stdio.h>
#define KX_MAX 16
#define KX_NAME 64
typedef struct { char algos[KX_MAX][KX_NAME]; int n; } KeyExch;
void keyexch_init(KeyExch *k) { if(k) memset(k,0,sizeof(*k)); }
int keyexch_add(KeyExch *k, const char *algo) {
    if(!k||k->n>=KX_MAX||!algo) return -1; snprintf(k->algos[k->n++],KX_NAME,"%s",algo); return 0;
}
int keyexch_negotiate(const KeyExch *client, const char *server_csv, char *out, int outlen) {
    if(!client||!server_csv) return -1;
    /* first client algo that appears in server list wins */
    for (int i=0;i<client->n;i++) {
        char needle[KX_NAME+2]; snprintf(needle,sizeof(needle),"%s",client->algos[i]);
        /* match as a comma-delimited token */
        const char *p=server_csv;
        while ((p=strstr(p,needle))) {
            char before = (p==server_csv)?',':*(p-1);
            char after = p[strlen(needle)];
            if ((before==','||p==server_csv) && (after==','||after==0)) {
                if (out) snprintf(out,outlen,"%s",client->algos[i]); return 0;
            }
            p+=strlen(needle);
        }
    }
    return -1;
}
int keyexch_count(const KeyExch *k) { return k?k->n:-1; }
