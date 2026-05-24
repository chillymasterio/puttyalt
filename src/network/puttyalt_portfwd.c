#include "puttyalt_portfwd.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void pfmgr_init(PortFwdMgr *mgr)
{
    memset(mgr, 0, sizeof(*mgr));
}

static int pfmgr_add(PortFwdMgr *mgr, PFType type, int lport,
                     const char *rhost, int rport, const char *label)
{
    if (mgr->count >= PF_MAX_RULES) return -1;
    PFRule *r = &mgr->rules[mgr->count];
    memset(r, 0, sizeof(*r));
    r->type = type;
    r->local_port = lport;
    if (rhost) snprintf(r->remote_host, PF_MAX_HOST, "%s", rhost);
    r->remote_port = rport;
    if (label) snprintf(r->label, sizeof(r->label), "%s", label);
    r->status = PF_INACTIVE;
    return mgr->count++;
}

int pfmgr_add_local(PortFwdMgr *mgr, int local_port,
                    const char *remote_host, int remote_port, const char *label)
{
    return pfmgr_add(mgr, PF_LOCAL, local_port, remote_host, remote_port, label);
}

int pfmgr_add_remote(PortFwdMgr *mgr, int remote_port,
                     const char *local_host, int local_port, const char *label)
{
    return pfmgr_add(mgr, PF_REMOTE, local_port, local_host, remote_port, label);
}

int pfmgr_add_dynamic(PortFwdMgr *mgr, int socks_port, const char *label)
{
    return pfmgr_add(mgr, PF_DYNAMIC, socks_port, NULL, 0, label);
}

int pfmgr_remove(PortFwdMgr *mgr, int index)
{
    if (index < 0 || index >= mgr->count) return -1;
    for (int i = index; i < mgr->count - 1; i++)
        mgr->rules[i] = mgr->rules[i + 1];
    mgr->count--;
    return 0;
}

int pfmgr_start(PortFwdMgr *mgr, int index)
{
    if (index < 0 || index >= mgr->count) return -1;
    mgr->rules[index].status = PF_ACTIVE;
    return 0;
}

int pfmgr_stop(PortFwdMgr *mgr, int index)
{
    if (index < 0 || index >= mgr->count) return -1;
    mgr->rules[index].status = PF_INACTIVE;
    return 0;
}

int pfmgr_start_all(PortFwdMgr *mgr)
{
    int started = 0;
    for (int i = 0; i < mgr->count; i++) {
        if (mgr->rules[i].auto_start && mgr->rules[i].status == PF_INACTIVE) {
            mgr->rules[i].status = PF_ACTIVE;
            started++;
        }
    }
    return started;
}

int pfmgr_stop_all(PortFwdMgr *mgr)
{
    int stopped = 0;
    for (int i = 0; i < mgr->count; i++) {
        if (mgr->rules[i].status == PF_ACTIVE) {
            mgr->rules[i].status = PF_INACTIVE;
            stopped++;
        }
    }
    return stopped;
}

int pfmgr_load(PortFwdMgr *mgr, const char *path)
{
    FILE *f = fopen(path, "r");
    char line[512];
    PFRule *cur = NULL;
    if (!f) return -1;
    pfmgr_init(mgr);
    while (fgets(line, sizeof(line), f)) {
        size_t len = strlen(line);
        while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r'))
            line[--len] = '\0';
        if (strcmp(line, "[forward]") == 0) {
            if (mgr->count >= PF_MAX_RULES) break;
            cur = &mgr->rules[mgr->count++];
            memset(cur, 0, sizeof(*cur));
            continue;
        }
        if (!cur) continue;
        if (strncmp(line, "type=", 5) == 0) cur->type = atoi(line + 5);
        else if (strncmp(line, "lport=", 6) == 0) cur->local_port = atoi(line + 6);
        else if (strncmp(line, "rhost=", 6) == 0)
            snprintf(cur->remote_host, PF_MAX_HOST, "%s", line + 6);
        else if (strncmp(line, "rport=", 6) == 0) cur->remote_port = atoi(line + 6);
        else if (strncmp(line, "label=", 6) == 0)
            snprintf(cur->label, sizeof(cur->label), "%s", line + 6);
        else if (strncmp(line, "auto=", 5) == 0) cur->auto_start = atoi(line + 5);
    }
    fclose(f);
    return 0;
}

int pfmgr_save(const PortFwdMgr *mgr, const char *path)
{
    FILE *f = fopen(path, "w");
    if (!f) return -1;
    for (int i = 0; i < mgr->count; i++) {
        const PFRule *r = &mgr->rules[i];
        fprintf(f, "[forward]\ntype=%d\nlport=%d\nrhost=%s\nrport=%d\n"
                   "label=%s\nauto=%d\n\n",
                r->type, r->local_port, r->remote_host, r->remote_port,
                r->label, r->auto_start);
    }
    fclose(f);
    return 0;
}

void pfmgr_record_bytes(PortFwdMgr *mgr, int index, long bytes)
{
    if (index >= 0 && index < mgr->count)
        mgr->rules[index].bytes_fwd += bytes;
}
