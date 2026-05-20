#include "puttyalt_restapi.h"
#include <string.h>
#include <stdio.h>

int rest_init(RestServer *rs, int port)
{
    memset(rs, 0, sizeof(*rs));
    rs->port = port > 0 ? port : REST_DEFAULT_PORT;
    rs->cors_enabled = 0;
    snprintf(rs->bind_addr, sizeof(rs->bind_addr), "127.0.0.1");
    return 0;
}

void rest_destroy(RestServer *rs)
{
    if (rs->running) rest_stop(rs);
    /* Zero API key */
    volatile char *p = (volatile char *)rs->api_key;
    for (int i = 0; i < 128; i++) p[i] = 0;
    memset(rs, 0, sizeof(*rs));
}

int rest_add_route(RestServer *rs, RestMethod method, const char *path,
                   int (*handler)(const char*, char*, int), int auth)
{
    if (rs->route_count >= REST_MAX_ROUTES) return -1;
    RestRoute *r = &rs->routes[rs->route_count];
    r->method = method;
    snprintf(r->path, sizeof(r->path), "%s", path);
    r->handler = handler;
    r->auth_required = auth;
    rs->route_count++;
    return rs->route_count - 1;
}

int rest_start(RestServer *rs)
{
    if (rs->running) return -1;
    /* In real implementation: bind TCP socket */
    rs->running = 1;
    return 0;
}

int rest_stop(RestServer *rs)
{
    rs->running = 0;
    return 0;
}

int rest_set_key(RestServer *rs, const char *api_key)
{
    if (api_key)
        snprintf(rs->api_key, sizeof(rs->api_key), "%s", api_key);
    else
        rs->api_key[0] = '\0';
    return 0;
}

int rest_process_request(RestServer *rs, RestMethod method,
                         const char *path, const char *body,
                         char *response, int resp_max)
{
    if (!rs->running) {
        snprintf(response, resp_max, "{\"error\":\"server not running\"}");
        return 503;
    }

    rs->request_count++;

    /* Find matching route */
    for (int i = 0; i < rs->route_count; i++) {
        RestRoute *r = &rs->routes[i];
        if (r->method != method) continue;
        if (strcmp(r->path, path) != 0) continue;

        if (r->auth_required && !rs->api_key[0]) {
            snprintf(response, resp_max, "{\"error\":\"unauthorized\"}");
            return 401;
        }

        if (r->handler) {
            int result = r->handler(body, response, resp_max);
            return result > 0 ? 200 : 500;
        }
        snprintf(response, resp_max, "{\"ok\":true}");
        return 200;
    }

    snprintf(response, resp_max, "{\"error\":\"not found\"}");
    return 404;
}
