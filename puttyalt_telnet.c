#include <string.h>
#include "puttyalt_telnet.h"

void telnet_init(TelnetState *ts, int cols, int rows)
{
    memset(ts, 0, sizeof(*ts));
    ts->cols = cols;
    ts->rows = rows;
    strncpy(ts->terminal_type, "xterm-256color", sizeof(ts->terminal_type) - 1);
}

int telnet_process(TelnetState *ts, const unsigned char *data, int len,
                   unsigned char *output, int *out_len)
{
    *out_len = 0;
    int consumed = 0;

    for (int i = 0; i < len; i++) {
        if (ts->in_subneg) {
            if (data[i] == TELNET_IAC && i + 1 < len && data[i+1] == TELNET_SE) {
                ts->in_subneg = 0;
                /* Handle subnegotiation */
                if (ts->sub_len > 0 && ts->sub_buf[0] == TELOPT_TTYPE) {
                    telnet_send_ttype(ts, output + *out_len, out_len);
                }
                i++; /* skip SE */
            } else if (ts->sub_len < (int)sizeof(ts->sub_buf)) {
                ts->sub_buf[ts->sub_len++] = data[i];
            }
            consumed++;
            continue;
        }

        if (data[i] == TELNET_IAC && i + 2 < len) {
            unsigned char cmd = data[i+1];
            unsigned char opt = data[i+2];

            switch (cmd) {
                case TELNET_DO:
                    if (opt == TELOPT_TTYPE || opt == TELOPT_NAWS || opt == TELOPT_SGA) {
                        output[(*out_len)++] = TELNET_IAC;
                        output[(*out_len)++] = TELNET_WILL;
                        output[(*out_len)++] = opt;
                        if (opt == TELOPT_NAWS) {
                            ts->naws_enabled = 1;
                            telnet_send_naws(ts, ts->cols, ts->rows,
                                           output + *out_len, out_len);
                        }
                        if (opt == TELOPT_SGA) ts->sga_enabled = 1;
                    } else {
                        output[(*out_len)++] = TELNET_IAC;
                        output[(*out_len)++] = TELNET_WONT;
                        output[(*out_len)++] = opt;
                    }
                    break;
                case TELNET_WILL:
                    if (opt == TELOPT_ECHO) {
                        ts->echo_local = 0;
                        output[(*out_len)++] = TELNET_IAC;
                        output[(*out_len)++] = TELNET_DO;
                        output[(*out_len)++] = opt;
                    }
                    break;
                case TELNET_SB:
                    ts->in_subneg = 1;
                    ts->sub_len = 0;
                    ts->sub_buf[ts->sub_len++] = opt;
                    break;
            }
            i += 2;
            consumed += 3;
        } else {
            consumed++;
        }
    }
    return consumed;
}

int telnet_send_naws(TelnetState *ts, int cols, int rows,
                     unsigned char *output, int *out_len)
{
    ts->cols = cols;
    ts->rows = rows;
    int p = *out_len;
    output[p++] = TELNET_IAC;
    output[p++] = TELNET_SB;
    output[p++] = TELOPT_NAWS;
    output[p++] = (cols >> 8) & 0xFF;
    output[p++] = cols & 0xFF;
    output[p++] = (rows >> 8) & 0xFF;
    output[p++] = rows & 0xFF;
    output[p++] = TELNET_IAC;
    output[p++] = TELNET_SE;
    *out_len = p;
    return 0;
}

int telnet_send_ttype(TelnetState *ts, unsigned char *output, int *out_len)
{
    int p = *out_len;
    output[p++] = TELNET_IAC;
    output[p++] = TELNET_SB;
    output[p++] = TELOPT_TTYPE;
    output[p++] = 0; /* IS */
    int tlen = strlen(ts->terminal_type);
    memcpy(&output[p], ts->terminal_type, tlen);
    p += tlen;
    output[p++] = TELNET_IAC;
    output[p++] = TELNET_SE;
    *out_len = p;
    ts->ttype_sent = 1;
    return 0;
}
