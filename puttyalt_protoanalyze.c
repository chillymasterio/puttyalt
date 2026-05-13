#include "puttyalt_protoanalyze.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

void pa_init(ProtoAnalyzer *pa)
{
    memset(pa, 0, sizeof(*pa));
    pa->filter_proto = PA_UNKNOWN;
    pa->decode_enabled = 1;
}

int pa_capture(ProtoAnalyzer *pa, PAProtocol proto, PADirection dir,
               const unsigned char *data, int len)
{
    if (!pa->capturing) return -1;
    if (len > PA_MAX_DATA) len = PA_MAX_DATA;

    PAPacket *p = &pa->packets[pa->write_pos];
    memset(p, 0, sizeof(*p));
    p->protocol = proto;
    p->direction = dir;
    memcpy(p->data, data, len);
    p->data_len = len;
    
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    p->timestamp_ms = ts.tv_sec * 1000L + ts.tv_nsec / 1000000L;
    p->seq_num = pa->count;

    if (dir == PA_DIR_IN) { pa->bytes_in += len; pa->packets_in++; }
    else { pa->bytes_out += len; pa->packets_out++; }

    if (pa->decode_enabled) pa_decode_packet(p);

    pa->write_pos = (pa->write_pos + 1) % PA_MAX_PACKETS;
    if (pa->count < PA_MAX_PACKETS) pa->count++;
    return 0;
}

void pa_start(ProtoAnalyzer *pa) { pa->capturing = 1; }
void pa_stop(ProtoAnalyzer *pa) { pa->capturing = 0; }

void pa_clear(ProtoAnalyzer *pa)
{
    long bi = pa->bytes_in, bo = pa->bytes_out;
    long pi = pa->packets_in, po = pa->packets_out;
    pa->count = 0;
    pa->write_pos = 0;
    pa->bytes_in = bi; pa->bytes_out = bo;
    pa->packets_in = pi; pa->packets_out = po;
}

void pa_set_filter(ProtoAnalyzer *pa, PAProtocol proto, PADirection dir)
{
    pa->filter_proto = proto;
    pa->filter_dir = dir;
}

int pa_get_filtered(const ProtoAnalyzer *pa, int *indices, int max)
{
    int n = 0;
    int start = pa->count >= PA_MAX_PACKETS ? pa->write_pos : 0;
    int total = pa->count < PA_MAX_PACKETS ? pa->count : PA_MAX_PACKETS;
    for (int i = 0; i < total && n < max; i++) {
        int idx = (start + i) % PA_MAX_PACKETS;
        const PAPacket *p = &pa->packets[idx];
        if (pa->filter_proto != PA_UNKNOWN && p->protocol != pa->filter_proto)
            continue;
        indices[n++] = idx;
    }
    return n;
}

void pa_decode_packet(PAPacket *pkt)
{
    int pos = 0, max = (int)sizeof(pkt->decoded) - 1;
    for (int i = 0; i < pkt->data_len && pos < max - 4; i++) {
        unsigned char c = pkt->data[i];
        if (isprint(c)) pkt->decoded[pos++] = (char)c;
        else { pos += snprintf(pkt->decoded + pos, max - pos, "\\x%02X", c); }
    }
    pkt->decoded[pos] = '\0';
}

int pa_export_text(const ProtoAnalyzer *pa, const char *path)
{
    FILE *f = fopen(path, "w");
    if (!f) return -1;
    static const char *protos[] = {"SSH","TELNET","RAW","SERIAL","?"};
    int start = pa->count >= PA_MAX_PACKETS ? pa->write_pos : 0;
    int total = pa->count < PA_MAX_PACKETS ? pa->count : PA_MAX_PACKETS;
    fprintf(f, "# Protocol Analyzer Export\n# Total: %d packets, %ld in / %ld out bytes\n\n",
            total, pa->bytes_in, pa->bytes_out);
    for (int i = 0; i < total; i++) {
        int idx = (start + i) % PA_MAX_PACKETS;
        const PAPacket *p = &pa->packets[idx];
        fprintf(f, "#%d [%s] %s %d bytes: %s\n",
                p->seq_num, protos[p->protocol],
                p->direction == PA_DIR_IN ? "<<<" : ">>>",
                p->data_len, p->decoded);
    }
    fclose(f);
    return 0;
}

int pa_export_pcap(const ProtoAnalyzer *pa, const char *path)
{
    /* Simplified pcap-like format */
    FILE *f = fopen(path, "wb");
    if (!f) return -1;
    unsigned int magic = 0xA1B2C3D4;
    unsigned short ver_maj = 2, ver_min = 4;
    unsigned int snaplen = PA_MAX_DATA, linktype = 147; /* USER0 */
    int zero = 0;
    fwrite(&magic, 4, 1, f);
    fwrite(&ver_maj, 2, 1, f);
    fwrite(&ver_min, 2, 1, f);
    fwrite(&zero, 4, 1, f);
    fwrite(&zero, 4, 1, f);
    fwrite(&snaplen, 4, 1, f);
    fwrite(&linktype, 4, 1, f);

    int start = pa->count >= PA_MAX_PACKETS ? pa->write_pos : 0;
    int total = pa->count < PA_MAX_PACKETS ? pa->count : PA_MAX_PACKETS;
    for (int i = 0; i < total; i++) {
        int idx = (start + i) % PA_MAX_PACKETS;
        const PAPacket *p = &pa->packets[idx];
        unsigned int ts_sec = (unsigned int)(p->timestamp_ms / 1000);
        unsigned int ts_usec = (unsigned int)((p->timestamp_ms % 1000) * 1000);
        unsigned int caplen = (unsigned int)p->data_len;
        fwrite(&ts_sec, 4, 1, f);
        fwrite(&ts_usec, 4, 1, f);
        fwrite(&caplen, 4, 1, f);
        fwrite(&caplen, 4, 1, f);
        fwrite(p->data, 1, p->data_len, f);
    }
    fclose(f);
    return 0;
}
