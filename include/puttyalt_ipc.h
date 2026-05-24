#ifndef PUTTYALT_IPC_H
#define PUTTYALT_IPC_H

#define IPC_MAX_MSG_LEN    4096
#define IPC_PIPE_NAME      "\\\\.\\pipe\\puttyalt_ipc"
#define IPC_UNIX_SOCK      "/tmp/puttyalt.sock"

typedef enum {
    IPC_CMD_OPEN_SESSION = 0,
    IPC_CMD_CLOSE_SESSION,
    IPC_CMD_SWITCH_TAB,
    IPC_CMD_RUN_COMMAND,
    IPC_CMD_GET_STATUS,
    IPC_CMD_BROADCAST,
    IPC_CMD_QUIT
} IPCCommand;

typedef struct IPCMessage {
    IPCCommand cmd;
    char payload[IPC_MAX_MSG_LEN];
    int payload_len;
    int response_expected;
} IPCMessage;

typedef struct IPCServer {
    int fd;
    int running;
    int client_count;
} IPCServer;

int  ipc_server_start(IPCServer *srv);
void ipc_server_stop(IPCServer *srv);
int  ipc_server_poll(IPCServer *srv, IPCMessage *msg);
int  ipc_send_command(IPCCommand cmd, const char *payload);

#endif
