#ifndef PUTTYALT_SERIAL_H
#define PUTTYALT_SERIAL_H

#define SERIAL_MAX_PORTS  16
#define SERIAL_DEV_LEN    128

typedef enum {
    SERIAL_PARITY_NONE = 0,
    SERIAL_PARITY_ODD,
    SERIAL_PARITY_EVEN,
    SERIAL_PARITY_MARK,
    SERIAL_PARITY_SPACE
} SerialParity;

typedef enum {
    SERIAL_FLOW_NONE = 0,
    SERIAL_FLOW_XONXOFF,
    SERIAL_FLOW_RTSCTS,
    SERIAL_FLOW_DSRDTR
} SerialFlowControl;

typedef struct SerialConfig {
    char device[SERIAL_DEV_LEN];
    int baud_rate;
    int data_bits;       /* 5, 6, 7, 8 */
    int stop_bits;       /* 1, 2 */
    SerialParity parity;
    SerialFlowControl flow;
    int local_echo;
    int line_ending;     /* 0=CR, 1=LF, 2=CRLF */
} SerialConfig;

typedef struct SerialPort {
    SerialConfig config;
    int fd;              /* file descriptor / handle */
    int is_open;
    unsigned long bytes_rx;
    unsigned long bytes_tx;
} SerialPort;

int  serial_open(SerialPort *port, const SerialConfig *cfg);
void serial_close(SerialPort *port);
int  serial_write(SerialPort *port, const void *data, int len);
int  serial_read(SerialPort *port, void *buf, int buflen);
int  serial_set_baud(SerialPort *port, int baud);
int  serial_set_dtr(SerialPort *port, int state);
int  serial_set_rts(SerialPort *port, int state);
int  serial_list_ports(char ports[][SERIAL_DEV_LEN], int max_ports);

#endif
