/* puttyalt_mtu2.c - MTU discovery with path probing state. */
#include <string.h>
typedef struct {
    int current_mtu;
    int min_mtu;
    int max_mtu;
    int probe_size;
    int discovered;
} Mtu2;
void mtu2_init(Mtu2 *m) {
    if (!m) return;
    memset(m, 0, sizeof(*m));
    m->min_mtu = 576;
    m->max_mtu = 1500;
    m->current_mtu = 1500;
    m->probe_size = 1500;
}
int mtu2_probe_result(Mtu2 *m, int probe_size, int success) {
    if (!m) return -1;
    if (success) {
        if (probe_size > m->current_mtu) m->current_mtu = probe_size;
        m->min_mtu = probe_size;
    } else {
        m->max_mtu = probe_size - 1;
    }
    /* binary search next probe */
    if (m->max_mtu - m->min_mtu <= 8) {
        m->current_mtu = m->min_mtu;
        m->discovered = 1;
        return m->current_mtu;
    }
    m->probe_size = (m->min_mtu + m->max_mtu) / 2;
    return m->probe_size;
}
int mtu2_next_probe(const Mtu2 *m) {
    return m ? m->probe_size : -1;
}
int mtu2_effective(const Mtu2 *m) {
    return m ? m->current_mtu : -1;
}
int mtu2_is_discovered(const Mtu2 *m) {
    return m ? m->discovered : -1;
}
