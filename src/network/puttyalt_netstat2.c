/* puttyalt_netstat2.c - Aggregate per-connection network stats.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { long rx_bytes; long tx_bytes; long rx_packets; long tx_packets; long errors; } NetStat2;
void nst_init(NetStat2 *n){ if(n)*n=(NetStat2){0,0,0,0,0}; }
void nst_rx(NetStat2 *n,long bytes){ if(n){n->rx_bytes+=bytes;n->rx_packets++;} }
void nst_tx(NetStat2 *n,long bytes){ if(n){n->tx_bytes+=bytes;n->tx_packets++;} }
long nst_total(const NetStat2 *n){ return n?n->rx_bytes+n->tx_bytes:0; }
