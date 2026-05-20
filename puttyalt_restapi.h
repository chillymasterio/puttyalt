#ifndef PUTTYALT_RESTAPI_H
#define PUTTYALT_RESTAPI_H

#define REST_MAX_ROUTES   64
#define REST_MAX_PATH     128
#define REST_DEFAULT_PORT 8822

typedef enum {
    REST_GET,
    REST_POST,
    REST_PUT,
    REST_DELETE
} RestMethod;

typedef struct {
    RestMethod method;
    char path[REST_MAX_PATH];
    int (*handler)(const char *body, char *response, int resp_max);
    int auth_required;
} RestRoute;

typedef struct {
    RestRoute routes[REST_MAX_ROUTES];
    int route_count;
    int port;
    int running;
    char api_key[128];
    int request_count;
    int cors_enabled;
    char bind_addr[64];
} RestServer;

int  rest_init(RestServer *rs, int port);
void rest_destroy(RestServer *rs);
int  rest_add_route(RestServer *rs, RestMethod method, const char *path,
                    int (*handler)(const char*, char*, int), int auth);
int  rest_start(RestServer *rs);
int  rest_stop(RestServer *rs);
int  rest_set_key(RestServer *rs, const char *api_key);
int  rest_process_request(RestServer *rs, RestMethod method,
                          const char *path, const char *body,
                          char *response, int resp_max);

#endif
