/*
 * puttyalt_quickconnect.h: Quick-connect URI parser.
 *
 * Parses connection strings like "ssh://user@host:port" or
 * "user@host" into structured connection parameters.  Powers the
 * Ctrl+K quick-connect bar and command-line URI handling.
 */

#ifndef PUTTYALT_QUICKCONNECT_H
#define PUTTYALT_QUICKCONNECT_H

#define QC_MAX_HOST     256
#define QC_MAX_USER     128
#define QC_MAX_SCHEME   16

typedef enum {
    QC_PROTO_SSH,
    QC_PROTO_TELNET,
    QC_PROTO_RAW,
    QC_PROTO_SERIAL,
    QC_PROTO_UNKNOWN
} QCProtocol;

typedef struct QuickConnectParams {
    char       host[QC_MAX_HOST];
    char       user[QC_MAX_USER];
    int        port;            /* 0 = use default for protocol */
    QCProtocol protocol;
    int        valid;           /* 1 if parsing succeeded */
} QuickConnectParams;

/* Parse a connection string into params.
 * Accepted formats:
 *   ssh://user@host:port
 *   telnet://host:port
 *   user@host:port
 *   user@host
 *   host:port
 *   host
 * Returns 1 on success, 0 on failure. */
int quickconnect_parse(const char *input, QuickConnectParams *out);

/* Get default port for a protocol */
int quickconnect_default_port(QCProtocol proto);

/* Format params back into a display string */
int quickconnect_format(const QuickConnectParams *p, char *buf, int bufsize);

#endif /* PUTTYALT_QUICKCONNECT_H */
