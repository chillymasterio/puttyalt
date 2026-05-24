#include "puttyalt_webhooks.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int webhook_init(WebhookManager *wm)
{
    memset(wm, 0, sizeof(*wm));
    wm->global_enabled = 1;
    wm->async_mode = 1;
    snprintf(wm->user_agent, sizeof(wm->user_agent),
             "PuttyAlt-Webhook/2.0");
    return 0;
}

void webhook_destroy(WebhookManager *wm)
{
    /* Zero secrets for security */
    for (int i = 0; i < wm->hook_count; i++) {
        volatile char *p = (volatile char *)wm->hooks[i].secret;
        for (int j = 0; j < 128; j++) p[j] = 0;
    }
    memset(wm, 0, sizeof(*wm));
}

int webhook_add(WebhookManager *wm, const char *name, const char *url,
                WebhookEvent event)
{
    if (wm->hook_count >= WH_MAX_HOOKS) return -1;
    if (!url || !url[0]) return -1;

    WebhookDef *h = &wm->hooks[wm->hook_count];
    memset(h, 0, sizeof(*h));
    snprintf(h->name, sizeof(h->name), "%s", name ? name : "hook");
    snprintf(h->url, sizeof(h->url), "%s", url);
    h->event = event;
    h->enabled = 1;
    h->retry_count = 3;
    h->timeout_ms = 5000;

    wm->hook_count++;
    return wm->hook_count - 1;
}

int webhook_remove(WebhookManager *wm, int idx)
{
    if (idx < 0 || idx >= wm->hook_count) return -1;
    /* Zero secret before removing */
    volatile char *p = (volatile char *)wm->hooks[idx].secret;
    for (int j = 0; j < 128; j++) p[j] = 0;

    for (int i = idx; i < wm->hook_count - 1; i++)
        wm->hooks[i] = wm->hooks[i + 1];
    wm->hook_count--;
    return 0;
}

static int webhook_check_timeout(const WebhookDef *h, long now)
{
    /* If last trigger was within the timeout window, consider it timed out
     * (in a real async implementation this would check pending HTTP state) */
    if (h->last_triggered > 0 && h->timeout_ms > 0) {
        long deadline = h->last_triggered + (h->timeout_ms / 1000);
        if (now > deadline && h->last_status == 0) {
            return 1; /* timed out */
        }
    }
    return 0;
}

int webhook_fire(WebhookManager *wm, WebhookEvent event,
                 const char *payload)
{
    if (!wm || !wm->global_enabled) return 0;
    int fired = 0;
    long now = (long)time(NULL);

    for (int i = 0; i < wm->hook_count; i++) {
        WebhookDef *h = &wm->hooks[i];
        if (!h->enabled || h->event != event) continue;

        /* Skip hooks that are still in timeout/pending state */
        if (webhook_check_timeout(h, now)) {
            h->last_status = 408; /* Request Timeout */
            continue;
        }

        h->last_triggered = now;
        h->total_fires++;

        /* In real implementation: HTTP POST to h->url
         * with payload as body and HMAC-SHA256 signature
         * using h->secret as key, respecting h->timeout_ms */
        (void)payload;
        h->last_status = 200;  /* simulated success */
        fired++;
    }

    return fired;
}

int webhook_set_secret(WebhookManager *wm, int idx, const char *secret)
{
    if (idx < 0 || idx >= wm->hook_count) return -1;
    if (secret)
        snprintf(wm->hooks[idx].secret, sizeof(wm->hooks[idx].secret),
                 "%s", secret);
    else
        wm->hooks[idx].secret[0] = '\0';
    return 0;
}

int webhook_test(WebhookManager *wm, int idx)
{
    if (idx < 0 || idx >= wm->hook_count) return -1;
    return webhook_fire(wm, wm->hooks[idx].event, "{\"test\": true}");
}

int webhook_load(WebhookManager *wm, const char *path)
{
    if (!wm || !path || !path[0]) return -1;
    FILE *f = fopen(path, "r");
    if (!f) return -1;
    char line[1024];
    while (fgets(line, sizeof(line), f)) {
        size_t len = strlen(line);
        while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r'))
            line[--len] = '\0';
        if (!line[0] || line[0] == '#') continue;
        /* Format: name|url|event_num */
        char *p1 = strchr(line, '|');
        if (!p1) continue;
        *p1++ = '\0';
        char *p2 = strchr(p1, '|');
        if (!p2) continue;
        *p2++ = '\0';
        int ev = atoi(p2);
        if (ev >= 0 && ev <= 5)
            webhook_add(wm, line, p1, (WebhookEvent)ev);
    }
    fclose(f);
    return 0;
}

int webhook_save(const WebhookManager *wm, const char *path)
{
    if (!wm || !path || !path[0]) return -1;
    FILE *f = fopen(path, "w");
    if (!f) return -1;
    fprintf(f, "# PuttyAlt Webhooks\n");
    for (int i = 0; i < wm->hook_count; i++) {
        const WebhookDef *h = &wm->hooks[i];
        fprintf(f, "%s|%s|%d\n", h->name, h->url, (int)h->event);
    }
    fclose(f);
    return 0;
}
