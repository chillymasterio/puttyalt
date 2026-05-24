#ifndef PUTTYALT_PROXY_DETECT_H
#define PUTTYALT_PROXY_DETECT_H

typedef struct {
    int enabled;
    int initialized;
    void *data;
    int data_size;
} Proxy_detectState;

int  proxy_detect_init(Proxy_detectState *s);
int  proxy_detect_update(Proxy_detectState *s);
void proxy_detect_destroy(Proxy_detectState *s);

#endif
