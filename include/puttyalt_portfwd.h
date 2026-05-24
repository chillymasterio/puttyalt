#ifndef PUTTYALT_PORTFWD_H
#define PUTTYALT_PORTFWD_H

#define PF_MAX_RULES    64
#define PF_MAX_HOST     256

typedef enum {
    PF_LOCAL = 0,
    PF_REMOTE,
    PF_DYNAMIC
} PFType;

typedef enum {
    PF_INACTIVE = 0,
    PF_ACTIVE,
    PF_ERROR
} PFStatus;

typedef struct {
    PFType  type;
    int     local_port;
    char    remote_host[PF_MAX_HOST];
    int     remote_port;
    char    bind_addr[64];
    PFStatus status;
    long    bytes_fwd;
    int     connections;
    int     auto_start;
    char    label[64];
} PFRule;

typedef struct {
    PFRule rules[PF_MAX_RULES];
    int    count;
} PortFwdMgr;

void pfmgr_init(PortFwdMgr *mgr);
int  pfmgr_add_local(PortFwdMgr *mgr, int local_port,
                     const char *remote_host, int remote_port, const char *label);
int  pfmgr_add_remote(PortFwdMgr *mgr, int remote_port,
                      const char *local_host, int local_port, const char *label);
int  pfmgr_add_dynamic(PortFwdMgr *mgr, int socks_port, const char *label);
int  pfmgr_remove(PortFwdMgr *mgr, int index);
int  pfmgr_start(PortFwdMgr *mgr, int index);
int  pfmgr_stop(PortFwdMgr *mgr, int index);
int  pfmgr_start_all(PortFwdMgr *mgr);
int  pfmgr_stop_all(PortFwdMgr *mgr);
int  pfmgr_load(PortFwdMgr *mgr, const char *path);
int  pfmgr_save(const PortFwdMgr *mgr, const char *path);
void pfmgr_record_bytes(PortFwdMgr *mgr, int index, long bytes);

#endif
