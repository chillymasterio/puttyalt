/* puttyalt_packetlog.c - SSH packet logging (type counts + sizes). */
#include <string.h>
#include <stdint.h>
#define PL_TYPES 64
typedef struct { uint32_t type_counts[PL_TYPES]; uint64_t total_sent, total_recv; uint64_t packet_count; } PacketLog;
void packetlog_init(PacketLog *p) { if(p) memset(p,0,sizeof(*p)); }
void packetlog_record(PacketLog *p, int type, int size, int sent) {
    if(!p) return;
    if (type>=0 && type<PL_TYPES) p->type_counts[type]++;
    if (sent) p->total_sent+=size; else p->total_recv+=size;
    p->packet_count++;
}
uint32_t packetlog_type_count(const PacketLog *p, int type) {
    return (p&&type>=0&&type<PL_TYPES)?p->type_counts[type]:0;
}
int packetlog_most_common(const PacketLog *p) {
    if(!p) return -1;
    int best=-1; uint32_t bc=0;
    for (int i=0;i<PL_TYPES;i++) if (p->type_counts[i]>bc) { bc=p->type_counts[i]; best=i; }
    return best;
}
uint64_t packetlog_total(const PacketLog *p) { return p?(p->total_sent+p->total_recv):0; }
