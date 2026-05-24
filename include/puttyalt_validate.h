#ifndef PUTTYALT_VALIDATE_H
#define PUTTYALT_VALIDATE_H

typedef enum {
    VALID_OK,
    VALID_EMPTY,
    VALID_TOO_LONG,
    VALID_INVALID_CHARS,
    VALID_OUT_OF_RANGE,
    VALID_INVALID_FORMAT
} ValidResult;

ValidResult validate_hostname(const char *host);
ValidResult validate_port(const char *port_str, int *port_out);
ValidResult validate_username(const char *user);
ValidResult validate_path(const char *path);
ValidResult validate_ip4(const char *ip);
ValidResult validate_ip6(const char *ip);
const char *validate_error_str(ValidResult r);

#endif
