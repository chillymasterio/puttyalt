#include <string.h>
#include <ctype.h>
#include "puttyalt_escparse.h"

enum { ST_GROUND, ST_ESC, ST_CSI_PARAM, ST_CSI_INTER, ST_OSC, ST_DCS };

void esc_parser_init(EscParser *ep)
{
    memset(ep, 0, sizeof(*ep));
}

void esc_parser_reset(EscParser *ep)
{
    memset(&ep->current, 0, sizeof(ep->current));
    ep->buf_len = 0;
    ep->state = ST_GROUND;
}

int esc_parser_feed(EscParser *ep, unsigned char byte)
{
    switch (ep->state) {
    case ST_GROUND:
        if (byte == 0x1B) {
            esc_parser_reset(ep);
            ep->state = ST_ESC;
        }
        return 0;

    case ST_ESC:
        if (byte == '[') {
            ep->current.type = ESC_CSI;
            ep->state = ST_CSI_PARAM;
        } else if (byte == ']') {
            ep->current.type = ESC_OSC;
            ep->state = ST_OSC;
        } else if (byte == 'P') {
            ep->current.type = ESC_DCS;
            ep->state = ST_DCS;
        } else if (byte >= 0x40 && byte <= 0x7E) {
            ep->current.type = ESC_SIMPLE;
            ep->current.final_byte = byte;
            ep->state = ST_GROUND;
            return 1; /* complete */
        } else {
            ep->state = ST_GROUND;
        }
        return 0;

    case ST_CSI_PARAM:
        if (byte == '?' || byte == '>') {
            ep->current.private_marker = byte;
        } else if (byte >= '0' && byte <= '9') {
            if (ep->current.num_params == 0) ep->current.num_params = 1;
            int idx = ep->current.num_params - 1;
            if (idx < ESC_MAX_PARAMS)
                ep->current.params[idx] = ep->current.params[idx] * 10 + (byte - '0');
        } else if (byte == ';') {
            if (ep->current.num_params < ESC_MAX_PARAMS)
                ep->current.num_params++;
        } else if (byte >= 0x40 && byte <= 0x7E) {
            ep->current.final_byte = byte;
            ep->state = ST_GROUND;
            return 1;
        } else if (byte >= 0x20 && byte <= 0x2F) {
            ep->state = ST_CSI_INTER;
        }
        return 0;

    case ST_CSI_INTER:
        if (byte >= 0x40 && byte <= 0x7E) {
            ep->current.final_byte = byte;
            ep->state = ST_GROUND;
            return 1;
        }
        return 0;

    case ST_OSC:
        if (byte == 0x07 || byte == 0x9C) { /* BEL or ST */
            ep->state = ST_GROUND;
            return 1;
        }
        if (byte == 0x1B) {
            /* Might be ESC \ (ST) */
            ep->state = ST_GROUND;
            return 1;
        }
        if (ep->current.osc_len < (int)sizeof(ep->current.osc_data) - 1)
            ep->current.osc_data[ep->current.osc_len++] = byte;
        return 0;

    case ST_DCS:
        if (byte == 0x9C || byte == 0x1B) {
            ep->state = ST_GROUND;
            return 1;
        }
        return 0;
    }
    return 0;
}

const EscSequence *esc_parser_result(const EscParser *ep)
{
    return &ep->current;
}
