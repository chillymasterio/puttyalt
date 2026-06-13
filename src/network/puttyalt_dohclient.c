/* puttyalt_dohclient.c - DNS-over-HTTPS request builder + response parser. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
typedef struct {
    char server[128];
    char hostname[160];
    int query_type;
    int response_code;
    char answer[64];
} DohClient;
void dohclient_init(DohClient *d, const char *server) {
    if (!d) return;
    memset(d, 0, sizeof(*d));
    snprintf(d->server, 128, "%s", server ? server : "https://cloudflare-dns.com/dns-query");
}
int dohclient_build_query(DohClient *d, const char *hostname, int type, char *out, int outlen) {
    if (!d || !hostname || !out) return -1;
    snprintf(d->hostname, 160, "%s", hostname);
    d->query_type = type;
    return snprintf(out, outlen, "%s?name=%s&type=%d", d->server, hostname,
                    type == 28 ? 28 : 1);
}
int dohclient_parse_answer(DohClient *d, const char *json) {
    if (!d || !json) return -1;
    const char *status = strstr(json, "\"Status\":");
    if (status) d->response_code = (int)atol(status + 9);
    const char *data = strstr(json, "\"data\":\"");
    if (data) {
        sscanf(data + 8, "%63[^\"]", d->answer);
        return 0;
    }
    return -1;
}
const char *dohclient_answer(const DohClient *d) {
    return d ? d->answer : "";
}
