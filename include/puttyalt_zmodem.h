/*
 * puttyalt_zmodem.h: ZMODEM file transfer protocol support.
 *
 * Detects ZMODEM transfer initiation sequences in terminal output
 * and automatically invokes the file transfer engine.
 */

#ifndef PUTTYALT_ZMODEM_H
#define PUTTYALT_ZMODEM_H

#define ZMODEM_DETECT_SEQ   "**\x18B00"
#define ZMODEM_DETECT_LEN   6
#define ZMODEM_BLOCK_SIZE   1024
#define ZMODEM_MAX_RETRIES  10
#define ZMODEM_TIMEOUT_SEC  30

typedef enum {
    ZM_IDLE = 0,
    ZM_DETECTING,
    ZM_RECEIVING,
    ZM_SENDING,
    ZM_COMPLETE,
    ZM_ERROR
} ZmodemState;

typedef struct ZmodemTransfer {
    ZmodemState state;
    char filename[256];
    unsigned long filesize;
    unsigned long bytes_transferred;
    int retries;
    int block_size;
    unsigned char detect_buf[ZMODEM_DETECT_LEN];
    int detect_pos;
} ZmodemTransfer;

void zmodem_init(ZmodemTransfer *zm);
int  zmodem_detect(ZmodemTransfer *zm, unsigned char byte);
int  zmodem_start_receive(ZmodemTransfer *zm, const char *download_dir);
int  zmodem_cancel(ZmodemTransfer *zm);
int  zmodem_progress_percent(const ZmodemTransfer *zm);

#endif /* PUTTYALT_ZMODEM_H */
