#ifndef PUTTYALT_PROTOANALYZE_H
#define PUTTYALT_PROTOANALYZE_H

#define PA_MAX_PACKETS  8192
#define PA_MAX_DATA     512

typedef enum {
    PA_SSH = 0,
    PA_TELNET,
    PA_RAW,
    PA_SERIAL,
    PA_UNKNOWN
} PAProtocol;

typedef enum {
    PA_DIR_IN = 0,
    PA_DIR_OUT
} PADirection;

typedef struct {
    PAProtocol  protocol;
    PADirection direction;
    unsigned char data[PA_MAX_DATA];
    int         data_len;
    long        timestamp_ms;
    int         seq_num;
    char        decoded[256];
} PAPacket;

typedef struct {
    PAPacket   packets[PA_MAX_PACKETS];
    int        count;
    int        write_pos;
    long       bytes_in;
    long       bytes_out;
    long       packets_in;
    long       packets_out;
    int        capturing;
    PAProtocol filter_proto;
    PADirection filter_dir;
    int        decode_enabled;
} ProtoAnalyzer;

void pa_init(ProtoAnalyzer *pa);
int  pa_capture(ProtoAnalyzer *pa, PAProtocol proto, PADirection dir,
                const unsigned char *data, int len);
void pa_start(ProtoAnalyzer *pa);
void pa_stop(ProtoAnalyzer *pa);
void pa_clear(ProtoAnalyzer *pa);
void pa_set_filter(ProtoAnalyzer *pa, PAProtocol proto, PADirection dir);
int  pa_get_filtered(const ProtoAnalyzer *pa, int *indices, int max);
int  pa_export_pcap(const ProtoAnalyzer *pa, const char *path);
int  pa_export_text(const ProtoAnalyzer *pa, const char *path);
void pa_decode_packet(PAPacket *pkt);

#endif
