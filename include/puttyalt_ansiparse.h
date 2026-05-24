#ifndef PUTTYALT_ANSIPARSE_H
#define PUTTYALT_ANSIPARSE_H
#include "puttyalt_termemu.h"

#define ANSI_MAX_PARAMS 16
#define ANSI_BUF_SIZE   256

typedef enum {
    ANSI_STATE_GROUND,
    ANSI_STATE_ESC,
    ANSI_STATE_CSI,
    ANSI_STATE_OSC,
    ANSI_STATE_DCS,
    ANSI_STATE_ST,
} AnsiParseState;

typedef struct {
    AnsiParseState state;
    int params[ANSI_MAX_PARAMS];
    int param_count;
    char intermediate;
    char final_char;
    char osc_buf[ANSI_BUF_SIZE];
    int osc_len;
    int private_mode; /* ? prefix */
} AnsiParser;

void ansi_init(AnsiParser *ap);
void ansi_feed(AnsiParser *ap, TermState *ts, uint8_t ch);
void ansi_process_csi(AnsiParser *ap, TermState *ts);
void ansi_process_sgr(AnsiParser *ap, TermState *ts);
void ansi_process_osc(AnsiParser *ap, TermState *ts);

#endif
