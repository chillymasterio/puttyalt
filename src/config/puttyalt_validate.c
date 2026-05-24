#include "puttyalt_validate.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

ValidResult validate_hostname(const char *host)
{
    if (!host || !host[0]) return VALID_EMPTY;
    size_t len = strlen(host);
    if (len > 253) return VALID_TOO_LONG;

    for (size_t i = 0; i < len; i++) {
        char c = host[i];
        if (!isalnum((unsigned char)c) && c != '.' && c != '-' && c != ':' && c != '[' && c != ']')
            return VALID_INVALID_CHARS;
    }

    /* Check for consecutive dots or leading/trailing dots */
    if (host[0] == '.' || host[len-1] == '.') return VALID_INVALID_FORMAT;
    if (strstr(host, "..")) return VALID_INVALID_FORMAT;

    return VALID_OK;
}

ValidResult validate_port(const char *port_str, int *port_out)
{
    if (!port_str || !port_str[0]) return VALID_EMPTY;

    for (int i = 0; port_str[i]; i++) {
        if (!isdigit((unsigned char)port_str[i]))
            return VALID_INVALID_CHARS;
    }

    int port = atoi(port_str);
    if (port < 1 || port > 65535) return VALID_OUT_OF_RANGE;
    if (port_out) *port_out = port;
    return VALID_OK;
}

ValidResult validate_username(const char *user)
{
    if (!user || !user[0]) return VALID_EMPTY;
    size_t len = strlen(user);
    if (len > 64) return VALID_TOO_LONG;

    for (size_t i = 0; i < len; i++) {
        char c = user[i];
        if (!isalnum((unsigned char)c) && c != '_' && c != '-' && c != '.')
            return VALID_INVALID_CHARS;
    }
    return VALID_OK;
}

ValidResult validate_path(const char *path)
{
    if (!path || !path[0]) return VALID_EMPTY;
    size_t len = strlen(path);
    if (len > 4096) return VALID_TOO_LONG;

    /* Check for dangerous patterns */
    if (strstr(path, "..") && strstr(path, "/.."))
        return VALID_INVALID_FORMAT;

    return VALID_OK;
}

ValidResult validate_ip4(const char *ip)
{
    if (!ip || !ip[0]) return VALID_EMPTY;
    int parts[4] = {0};
    int n = 0;
    const char *p = ip;

    while (*p && n < 4) {
        if (!isdigit((unsigned char)*p)) {
            if (*p == '.' && n < 3) { n++; p++; continue; }
            return VALID_INVALID_CHARS;
        }
        parts[n] = parts[n] * 10 + (*p - '0');
        if (parts[n] > 255) return VALID_OUT_OF_RANGE;
        p++;
    }

    if (n != 3 || *p != '\0') return VALID_INVALID_FORMAT;
    return VALID_OK;
}

ValidResult validate_ip6(const char *ip)
{
    if (!ip || !ip[0]) return VALID_EMPTY;
    /* Basic IPv6 validation: check for hex and colons */
    int colons = 0;
    for (const char *p = ip; *p; p++) {
        if (*p == ':') colons++;
        else if (!isxdigit((unsigned char)*p))
            return VALID_INVALID_CHARS;
    }
    if (colons < 2 || colons > 7) return VALID_INVALID_FORMAT;
    return VALID_OK;
}

const char *validate_error_str(ValidResult r)
{
    switch (r) {
    case VALID_OK: return "OK";
    case VALID_EMPTY: return "Field is required";
    case VALID_TOO_LONG: return "Value too long";
    case VALID_INVALID_CHARS: return "Invalid characters";
    case VALID_OUT_OF_RANGE: return "Value out of range";
    case VALID_INVALID_FORMAT: return "Invalid format";
    }
    return "Unknown error";
}
