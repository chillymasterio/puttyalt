/* puttyalt_pqkem.c - Post-quantum KEM negotiation descriptor (ML-KEM / hybrid). */
#include <string.h>
#include <stdio.h>
#define PQ_MAX 8
#define PQ_NAME 48
enum pq_kem { PQ_NONE=0, PQ_MLKEM512=1, PQ_MLKEM768=2, PQ_MLKEM1024=3, PQ_X25519_MLKEM768=4 };
typedef struct { int kem; char name[PQ_NAME]; int classical_bits; int pq_level; } pq_entry;
typedef struct { pq_entry e[PQ_MAX]; int n; int selected; } PqKem;
void pqkem_init(PqKem *p) { if(p){ memset(p,0,sizeof(*p)); p->selected=-1; } }
int pqkem_offer(PqKem *p, int kem, const char *name, int classical_bits, int pq_level) {
    if(!p||p->n>=PQ_MAX||!name) return -1;
    pq_entry *e=&p->e[p->n++]; e->kem=kem; snprintf(e->name,PQ_NAME,"%s",name);
    e->classical_bits=classical_bits; e->pq_level=pq_level; return 0;
}
int pqkem_negotiate(PqKem *p) {
    if(!p) return -1;
    int best=-1, blevel=-1;
    for (int i=0;i<p->n;i++) if (p->e[i].pq_level>blevel) { blevel=p->e[i].pq_level; best=i; }
    p->selected=best; return best;
}
int pqkem_is_hybrid(const PqKem *p) {
    if(!p||p->selected<0) return 0;
    return p->e[p->selected].classical_bits>0 && p->e[p->selected].pq_level>0;
}
int pqkem_selected_kem(const PqKem *p) { return (p&&p->selected>=0)?p->e[p->selected].kem:PQ_NONE; }
int pqkem_count(const PqKem *p) { return p?p->n:-1; }
