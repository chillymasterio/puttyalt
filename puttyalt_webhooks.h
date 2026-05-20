#ifndef PUTTYALT_WEBHOOKS_H
#define PUTTYALT_WEBHOOKS_H

#define WH_MAX_HOOKS     32
#define WH_MAX_URL       512
#define WH_MAX_PAYLOAD   2048
#define WH_MAX_HEADER    256

typedef enum {
    WH_EVENT_CONNECT,
    WH_EVENT_DISCONNECT,
    WH_EVENT_AUTH_FAIL,
    WH_EVENT_IDLE,
    WH_EVENT_ERROR,
    WH_EVENT_CUSTOM
} WebhookEvent;

typedef struct {
    char name[64];
    char url[WH_MAX_URL];
    char secret[128];
    char headers[WH_MAX_HEADER];
    WebhookEvent event;
    int enabled;
    int retry_count;
    int timeout_ms;
    long last_triggered;
    int last_status;
    int total_fires;
    int total_failures;
} WebhookDef;

typedef struct {
    WebhookDef hooks[WH_MAX_HOOKS];
    int hook_count;
    int global_enabled;
    int async_mode;
    char user_agent[64];
} WebhookManager;

int  webhook_init(WebhookManager *wm);
void webhook_destroy(WebhookManager *wm);
int  webhook_add(WebhookManager *wm, const char *name, const char *url,
                 WebhookEvent event);
int  webhook_remove(WebhookManager *wm, int idx);
int  webhook_fire(WebhookManager *wm, WebhookEvent event,
                  const char *payload);
int  webhook_set_secret(WebhookManager *wm, int idx, const char *secret);
int  webhook_test(WebhookManager *wm, int idx);
int  webhook_load(WebhookManager *wm, const char *path);
int  webhook_save(const WebhookManager *wm, const char *path);

#endif
