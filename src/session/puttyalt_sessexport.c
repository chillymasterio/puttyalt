#include <string.h>
#include <stdio.h>
int sessexport_to_json(const char *name, const char *host, int port, const char *user, char *buf, int buflen) {
    return snprintf(buf, buflen, "{\"name\":\"%s\",\"host\":\"%s\",\"port\":%d,\"user\":\"%s\"}", name, host, port, user ? user : "");
}
int sessexport_to_csv_header(char *buf, int buflen) {
    return snprintf(buf, buflen, "name,host,port,user\n");
}
int sessexport_to_csv_row(const char *name, const char *host, int port, const char *user, char *buf, int buflen) {
    return snprintf(buf, buflen, "%s,%s,%d,%s\n", name, host, port, user ? user : "");
}
int sessexport_to_ssh_config(const char *name, const char *host, int port, const char *user, const char *key, char *buf, int buflen) {
    int pos = snprintf(buf, buflen, "Host %s\n  HostName %s\n  Port %d\n", name, host, port);
    if (user && user[0]) pos += snprintf(buf + pos, buflen - pos, "  User %s\n", user);
    if (key && key[0]) pos += snprintf(buf + pos, buflen - pos, "  IdentityFile %s\n", key);
    return pos;
}
