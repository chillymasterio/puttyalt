#ifndef PUTTYALT_ESCPARSE_H
#define PUTTYALT_ESCPARSE_H

#define ESC_MAX_PARAMS  16

typedef enum {
    ESC_NONE = 0,
    ESC_CSI,       /* Control Sequence Introducer */
    ESC_OSC,       /* Operating System Command */
    ESC_DCS,       /* Device Control String */
    ESC_SIMPLE     /* Simple ESC + letter */
} EscType;

typedef struct EscSequence {
    EscType type;
    char final_byte;
    int params[ESC_MAX_PARAMS];
    int num_params;
    int private_marker;   /* ? or > prefix */
    char osc_data[512];
    int osc_len;
} EscSequence;

typedef struct EscParser {
    int state;
    EscSequence current;
    unsigned char buf[1024];
    int buf_len;
} EscParser;

void esc_parser_init(EscParser *ep);
int  esc_parser_feed(EscParser *ep, unsigned char byte);
const EscSequence *esc_parser_result(const EscParser *ep);
void esc_parser_reset(EscParser *ep);

#endif
