/* puttyalt_tcpstate.c - TCP connection state machine names.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
enum { TCP_CLOSED, TCP_LISTEN, TCP_SYN_SENT, TCP_SYN_RECV, TCP_ESTABLISHED,
       TCP_FIN_WAIT1, TCP_FIN_WAIT2, TCP_CLOSE_WAIT, TCP_CLOSING, TCP_LAST_ACK, TCP_TIME_WAIT };
const char *ts2_name(int state) {
    static const char *names[] = {"CLOSED","LISTEN","SYN_SENT","SYN_RECV","ESTABLISHED",
        "FIN_WAIT1","FIN_WAIT2","CLOSE_WAIT","CLOSING","LAST_ACK","TIME_WAIT"};
    if (state < 0 || state > TCP_TIME_WAIT) return "INVALID";
    return names[state];
}
int ts2_is_connected(int state) { return state == TCP_ESTABLISHED; }
int ts2_is_closing(int state) {
    return state >= TCP_FIN_WAIT1 && state <= TCP_TIME_WAIT;
}
