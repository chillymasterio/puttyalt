#include <string.h>
#include <stdio.h>
#include "puttyalt_serial.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#endif

int serial_open(SerialPort *port, const SerialConfig *cfg)
{
    memset(port, 0, sizeof(*port));
    port->config = *cfg;
    port->fd = -1;

#ifndef _WIN32
    port->fd = open(cfg->device, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (port->fd < 0 || !port->is_open) return -1;

    struct termios tty;
    memset(&tty, 0, sizeof(tty));
    if (tcgetattr(port->fd, &tty) != 0) {
        close(port->fd);
        port->fd = -1;
        return -1;
    }

    speed_t speed;
    switch (cfg->baud_rate) {
        case 9600:   speed = B9600;   break;
        case 19200:  speed = B19200;  break;
        case 38400:  speed = B38400;  break;
        case 57600:  speed = B57600;  break;
        case 115200: speed = B115200; break;
        default:     speed = B9600;   break;
    }
    cfsetospeed(&tty, speed);
    cfsetispeed(&tty, speed);

    tty.c_cflag &= ~CSIZE;
    switch (cfg->data_bits) {
        case 5: tty.c_cflag |= CS5; break;
        case 6: tty.c_cflag |= CS6; break;
        case 7: tty.c_cflag |= CS7; break;
        default: tty.c_cflag |= CS8; break;
    }

    if (cfg->stop_bits == 2) tty.c_cflag |= CSTOPB;
    else tty.c_cflag &= ~CSTOPB;

    switch (cfg->parity) {
        case SERIAL_PARITY_ODD:  tty.c_cflag |= PARENB | PARODD; break;
        case SERIAL_PARITY_EVEN: tty.c_cflag |= PARENB; tty.c_cflag &= ~PARODD; break;
        default: tty.c_cflag &= ~PARENB; break;
    }

    if (cfg->flow == SERIAL_FLOW_RTSCTS) tty.c_cflag |= CRTSCTS;
    else tty.c_cflag &= ~CRTSCTS;

    tty.c_cflag |= CLOCAL | CREAD;
    tty.c_lflag = 0;
    tty.c_iflag = 0;
    tty.c_oflag = 0;
    tty.c_cc[VMIN] = 0;
    tty.c_cc[VTIME] = 1;

    tcsetattr(port->fd, TCSANOW, &tty);
#endif

    port->is_open = 1;
    return 0;
}

void serial_close(SerialPort *port)
{
    if (!port->is_open) return;
#ifndef _WIN32
    if (port->fd >= 0) close(port->fd);
#endif
    port->fd = -1;
    port->is_open = 0;
}

int serial_write(SerialPort *port, const void *data, int len)
{
    if (!port->is_open || port->fd < 0) return -1;
#ifndef _WIN32
    int n = write(port->fd, data, len);
    if (n > 0) port->bytes_tx += n;
    return n;
#else
    (void)data; (void)len;
    return -1;
#endif
}

int serial_read(SerialPort *port, void *buf, int buflen)
{
    if (!port->is_open || port->fd < 0) return -1;
#ifndef _WIN32
    int n = read(port->fd, buf, buflen);
    if (n > 0) port->bytes_rx += n;
    return n;
#else
    (void)buf; (void)buflen;
    return -1;
#endif
}

int serial_set_baud(SerialPort *port, int baud)
{
    port->config.baud_rate = baud;
    if (!port->is_open) return 0;
    /* Would re-apply termios here */
    return 0;
}

int serial_set_dtr(SerialPort *port, int state)
{
    if (!port->is_open || port->fd < 0) return -1;
#ifndef _WIN32
    int flags;
    ioctl(port->fd, TIOCMGET, &flags);
    if (state) flags |= TIOCM_DTR;
    else flags &= ~TIOCM_DTR;
    ioctl(port->fd, TIOCMSET, &flags);
#endif
    return 0;
}

int serial_set_rts(SerialPort *port, int state)
{
    if (!port->is_open || port->fd < 0) return -1;
#ifndef _WIN32
    int flags;
    ioctl(port->fd, TIOCMGET, &flags);
    if (state) flags |= TIOCM_RTS;
    else flags &= ~TIOCM_RTS;
    ioctl(port->fd, TIOCMSET, &flags);
#endif
    return 0;
}

int serial_list_ports(char ports[][SERIAL_DEV_LEN], int max_ports)
{
    int count = 0;
#ifndef _WIN32
    const char *patterns[] = {"/dev/ttyS", "/dev/ttyUSB", "/dev/ttyACM", NULL};
    for (int p = 0; patterns[p] && count < max_ports; p++) {
        for (int i = 0; i < 16 && count < max_ports; i++) {
            char path[SERIAL_DEV_LEN];
            snprintf(path, sizeof(path), "%s%d", patterns[p], i);
            if (access(path, F_OK) == 0) {
                strncpy(ports[count], path, SERIAL_DEV_LEN - 1);
                count++;
            }
        }
    }
#endif
    return count;
}
