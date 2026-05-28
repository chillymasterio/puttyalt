#include <string.h>
#include <stdio.h>
static int g_nodelay = 1; static int g_send_buffer = 0; static int g_recv_buffer = 0;
void tcp_set_nodelay(int on) { g_nodelay = on; }
int tcp_get_nodelay(void) { return g_nodelay; }
void tcp_set_buffers(int send_kb, int recv_kb) { g_send_buffer = send_kb * 1024; g_recv_buffer = recv_kb * 1024; }
int tcp_format_opts(char *buf, int buflen) {
    return snprintf(buf, buflen, "TCP_NODELAY=%s SendBuf=%dKB RecvBuf=%dKB",
        g_nodelay ? "on" : "off", g_send_buffer/1024, g_recv_buffer/1024);
}
