#ifndef PUTTYALT_TELNET_H
#define PUTTYALT_TELNET_H

/* Telnet protocol constants */
#define TELNET_IAC   255
#define TELNET_DONT  254
#define TELNET_DO    253
#define TELNET_WONT  252
#define TELNET_WILL  251
#define TELNET_SB    250
#define TELNET_SE    240

/* Common options */
#define TELOPT_ECHO         1
#define TELOPT_SGA          3
#define TELOPT_TTYPE        24
#define TELOPT_NAWS         31
#define TELOPT_LINEMODE     34
#define TELOPT_NEW_ENVIRON  39

typedef struct TelnetState {
    int echo_local;
    int sga_enabled;
    int naws_enabled;
    int ttype_sent;
    char terminal_type[64];
    int cols, rows;
    unsigned char sub_buf[256];
    int sub_len;
    int in_subneg;
} TelnetState;

void telnet_init(TelnetState *ts, int cols, int rows);
int  telnet_process(TelnetState *ts, const unsigned char *data, int len,
                    unsigned char *output, int *out_len);
int  telnet_send_naws(TelnetState *ts, int cols, int rows,
                      unsigned char *output, int *out_len);
int  telnet_send_ttype(TelnetState *ts, unsigned char *output, int *out_len);

#endif
