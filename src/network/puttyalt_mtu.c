#include <string.h>
#include <stdio.h>
typedef struct { int current_mtu; int min_mtu; int max_mtu; int auto_detect; } MtuConfig;
static MtuConfig g_mtu = {1400, 576, 9000, 1};
void mtu_set(int mtu) { if (mtu >= g_mtu.min_mtu && mtu <= g_mtu.max_mtu) g_mtu.current_mtu = mtu; }
int mtu_get(void) { return g_mtu.current_mtu; }
void mtu_auto(int enabled) { g_mtu.auto_detect = enabled; }
int mtu_format(char *buf, int buflen) {
    return snprintf(buf, buflen, "MTU: %d bytes%s", g_mtu.current_mtu, g_mtu.auto_detect ? " (auto)" : "");
}
