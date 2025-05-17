/*
 * puttyalt_zmodem.c: ZMODEM protocol detection and transfer stub.
 *
 * This provides the detection layer. The actual ZMODEM protocol
 * handling delegates to an external rz/sz binary or will be
 * implemented in a future version.
 */

#include <string.h>
#include <stdio.h>
#include "puttyalt_zmodem.h"

static const unsigned char zmodem_sig[] = { '*', '*', 0x18, 'B', '0', '0' };

void zmodem_init(ZmodemTransfer *zm)
{
    memset(zm, 0, sizeof(*zm));
    zm->state = ZM_IDLE;
    zm->block_size = ZMODEM_BLOCK_SIZE;
}

int zmodem_detect(ZmodemTransfer *zm, unsigned char byte)
{
    if (zm->state == ZM_RECEIVING || zm->state == ZM_SENDING)
        return 0; /* already in transfer */

    if (byte == zmodem_sig[zm->detect_pos]) {
        zm->detect_buf[zm->detect_pos] = byte;
        zm->detect_pos++;

        if (zm->detect_pos >= ZMODEM_DETECT_LEN) {
            zm->state = ZM_DETECTING;
            zm->detect_pos = 0;
            return 1; /* detected! */
        }
    } else {
        zm->detect_pos = 0;
        if (byte == zmodem_sig[0]) {
            zm->detect_buf[0] = byte;
            zm->detect_pos = 1;
        }
    }

    return 0;
}

int zmodem_start_receive(ZmodemTransfer *zm, const char *download_dir)
{
    if (zm->state != ZM_DETECTING)
        return -1;

    zm->state = ZM_RECEIVING;
    zm->bytes_transferred = 0;
    zm->retries = 0;

    /* In a full implementation, this would fork rz or handle
     * the ZMODEM protocol directly */
    (void)download_dir;
    return 0;
}

int zmodem_cancel(ZmodemTransfer *zm)
{
    if (zm->state == ZM_IDLE)
        return -1;

    zm->state = ZM_IDLE;
    zm->detect_pos = 0;
    zm->bytes_transferred = 0;
    return 0;
}

int zmodem_progress_percent(const ZmodemTransfer *zm)
{
    if (zm->filesize == 0)
        return 0;
    return (int)((zm->bytes_transferred * 100) / zm->filesize);
}
