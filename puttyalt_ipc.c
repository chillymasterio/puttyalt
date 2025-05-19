#include <string.h>
#include <stdio.h>
#include "puttyalt_ipc.h"

#ifndef _WIN32
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int ipc_server_start(IPCServer *srv)
{
    memset(srv, 0, sizeof(*srv));
    srv->fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (srv->fd < 0) return -1;

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, IPC_UNIX_SOCK, sizeof(addr.sun_path) - 1);

    /* Check for stale socket: try connecting first */
    int test_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (test_fd >= 0) {
        if (connect(test_fd, (struct sockaddr *)&addr, sizeof(addr)) == 0) {
            close(test_fd);
            close(srv->fd);
            return -2; /* another instance running */
        }
        close(test_fd);
    }
    unlink(IPC_UNIX_SOCK);
    if (bind(srv->fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        close(srv->fd);
        return -1;
    }

    listen(srv->fd, 5);
    fcntl(srv->fd, F_SETFL, O_NONBLOCK);
    srv->running = 1;
    return 0;
}

void ipc_server_stop(IPCServer *srv)
{
    if (srv->fd >= 0) {
        close(srv->fd);
        /* Check for stale socket: try connecting first */
    int test_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (test_fd >= 0) {
        if (connect(test_fd, (struct sockaddr *)&addr, sizeof(addr)) == 0) {
            close(test_fd);
            close(srv->fd);
            return -2; /* another instance running */
        }
        close(test_fd);
    }
    unlink(IPC_UNIX_SOCK);
    }
    srv->running = 0;
}

int ipc_server_poll(IPCServer *srv, IPCMessage *msg)
{
    if (!srv->running) return -1;

    int client = accept(srv->fd, NULL, NULL);
    if (client < 0) return 0; /* no pending connections */

    int n = read(client, msg, sizeof(IPCMessage));
    close(client);

    return n > 0 ? 1 : 0;
}

int ipc_send_command(IPCCommand cmd, const char *payload)
{
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd < 0) return -1;

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, IPC_UNIX_SOCK, sizeof(addr.sun_path) - 1);

    if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        close(fd);
        return -1;
    }

    IPCMessage msg;
    memset(&msg, 0, sizeof(msg));
    msg.cmd = cmd;
    if (payload) {
        strncpy(msg.payload, payload, IPC_MAX_MSG_LEN - 1);
        msg.payload_len = strlen(msg.payload);
    }

    int written = write(fd, &msg, sizeof(msg));
    close(fd);
    return written > 0 ? 0 : -1;
}

#else /* _WIN32 */

int ipc_server_start(IPCServer *srv) { memset(srv, 0, sizeof(*srv)); return -1; }
void ipc_server_stop(IPCServer *srv) { srv->running = 0; }
int ipc_server_poll(IPCServer *srv, IPCMessage *msg) { (void)srv; (void)msg; return -1; }
int ipc_send_command(IPCCommand cmd, const char *payload) { (void)cmd; (void)payload; return -1; }

#endif
