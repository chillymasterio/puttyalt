/*
 * puttyalt.h: PuttyAlt version and branding definitions.
 *
 * This header centralises all PuttyAlt-specific constants so that
 * the upstream PuTTY code can remain largely unmodified.
 */

#ifndef PUTTYALT_H
#define PUTTYALT_H

#define PUTTYALT_VERSION_MAJOR  0
#define PUTTYALT_VERSION_MINOR  1
#define PUTTYALT_VERSION_PATCH  0

#define PUTTYALT_VERSION_STR    "0.1.0"
#define PUTTYALT_NAME           "PuttyAlt"
#define PUTTYALT_APPNAME        "PuttyAlt"

/* Upstream base */
#define PUTTYALT_UPSTREAM       "PuTTY 0.83"

/* Feature flags — enable/disable PuttyAlt enhancements */
#define PUTTYALT_PORTABLE_MODE  1   /* Registry-free portable config */
#define PUTTYALT_AUTO_RECONNECT 1   /* Auto-reconnect on connection drop */
#define PUTTYALT_REMEMBER_SIZE  1   /* Remember window position/size */
#define PUTTYALT_SESSION_MGR    1   /* Session manager with folders */
#define PUTTYALT_SNIPPETS       1   /* Command snippet store */
#define PUTTYALT_HEALTH_MON     1   /* Connection health monitor */
#define PUTTYALT_LOG_ROTATE     1   /* Automatic log rotation */
#define PUTTYALT_CLIP_HISTORY   1   /* Clipboard history ring */
#define PUTTYALT_TRIGGERS       1   /* Terminal output triggers */
#define PUTTYALT_BROADCAST      1   /* Multi-input broadcast */
#define PUTTYALT_KEY_MANAGER    1   /* SSH key inventory */
#define PUTTYALT_TERM_SEARCH    1   /* Scrollback search */
#define PUTTYALT_ENV_INDICATOR  1   /* Environment colour stripes */
#define PUTTYALT_QUICK_CONNECT  1   /* URI-based quick connect */

#endif /* PUTTYALT_H */

/* v0.2.0 feature flags */
#define PUTTYALT_TABS            1   /* Multi-session tabs */
#define PUTTYALT_SFTP_PANEL      1   /* Split-pane SFTP browser */
#define PUTTYALT_CRED_STORE      1   /* Encrypted credential store */
#define PUTTYALT_THEMES          1   /* Colour theme engine */
#define PUTTYALT_IMPORT          1   /* Session import */
#define PUTTYALT_SHORTCUTS       1   /* Custom keyboard shortcuts */
#define PUTTYALT_NOTIFY          1   /* Desktop notifications */
#define PUTTYALT_PROXY_DETECT    1   /* Auto proxy detection */
#define PUTTYALT_TRANSFER        1   /* File transfer progress */
#define PUTTYALT_LATENCY         1   /* Connection latency tracker */
#define PUTTYALT_CMD_HISTORY     1   /* Cross-session command history */
#define PUTTYALT_STATUSBAR       1   /* Information status bar */
#define PUTTYALT_TIMEOUT         1   /* Per-session timeouts */
#define PUTTYALT_ALIASES         1   /* Hostname aliases */
