/*
 * puttyalt.h: PuttyAlt version and branding definitions.
 */

#ifndef PUTTYALT_H
#define PUTTYALT_H

#define PUTTYALT_VERSION_MAJOR  2
#define PUTTYALT_VERSION_MINOR  0
#define PUTTYALT_VERSION_PATCH  1

#define PUTTYALT_VERSION_STR    "2.0.1"
#define PUTTYALT_NAME           "PuttyAlt"
#define PUTTYALT_APPNAME        "PuttyAlt"
#define PUTTYALT_UPSTREAM       "PuTTY 0.83"

/* v0.1.0 — Core enhancements */
#define PUTTYALT_PORTABLE_MODE  1
#define PUTTYALT_AUTO_RECONNECT 1
#define PUTTYALT_REMEMBER_SIZE  1
#define PUTTYALT_SESSION_MGR    1
#define PUTTYALT_SNIPPETS       1
#define PUTTYALT_HEALTH_MON     1
#define PUTTYALT_LOG_ROTATE     1
#define PUTTYALT_CLIP_HISTORY   1
#define PUTTYALT_TRIGGERS       1
#define PUTTYALT_BROADCAST      1
#define PUTTYALT_KEY_MANAGER    1
#define PUTTYALT_TERM_SEARCH    1
#define PUTTYALT_ENV_INDICATOR  1
#define PUTTYALT_QUICK_CONNECT  1

/* v0.2.0 — Session & UI */
#define PUTTYALT_TABS            1
#define PUTTYALT_SFTP_PANEL      1
#define PUTTYALT_CRED_STORE      1
#define PUTTYALT_THEMES          1
#define PUTTYALT_IMPORT          1
#define PUTTYALT_SHORTCUTS       1
#define PUTTYALT_NOTIFY          1
#define PUTTYALT_PROXY_DETECT    1
#define PUTTYALT_TRANSFER        1
#define PUTTYALT_LATENCY         1
#define PUTTYALT_CMD_HISTORY     1
#define PUTTYALT_STATUSBAR       1
#define PUTTYALT_TIMEOUT         1
#define PUTTYALT_ALIASES         1
#define PUTTYALT_PINNING         1
#define PUTTYALT_JUMPHOST        1

/* v0.3.0 — Power features */
#define PUTTYALT_SPLIT_VIEW      1
#define PUTTYALT_MACRO           1
#define PUTTYALT_LOGGING_V2      1
#define PUTTYALT_MOUSE_GESTURES  1
#define PUTTYALT_ZMODEM          1
#define PUTTYALT_AUTO_COMPLETE   1
#define PUTTYALT_SESSION_LOCK    1
#define PUTTYALT_BANDWIDTH       1

/* v0.4.0 — Infrastructure & networking */
#define PUTTYALT_WORKSPACE       1
#define PUTTYALT_TUNNEL_MGR      1
#define PUTTYALT_SERIAL          1
#define PUTTYALT_FIND_REPLACE    1
#define PUTTYALT_SESSION_TAGS    1
#define PUTTYALT_EXPORT_V2       1
#define PUTTYALT_CONN_POOL       1
#define PUTTYALT_URL_HANDLER     1
#define PUTTYALT_SCP             1
#define PUTTYALT_NET_DIAG        1
#define PUTTYALT_TEMPLATES       1
#define PUTTYALT_COLOR_PICKER    1
#define PUTTYALT_FONT_MGR        1
#define PUTTYALT_STARTUP_CMD     1
#define PUTTYALT_LAYOUTS         1
#define PUTTYALT_MIGRATION       1
#define PUTTYALT_PLUGIN          1
#define PUTTYALT_EVENT_BUS       1
#define PUTTYALT_IPC             1
#define PUTTYALT_AUTO_UPDATE     1
#define PUTTYALT_BOOKMARKS       1
#define PUTTYALT_SHARED_CLIP     1
#define PUTTYALT_KEEP_ALIVE      1
#define PUTTYALT_BELL_V2         1
#define PUTTYALT_AGENT_FWD       1
#define PUTTYALT_X11_FWD         1
#define PUTTYALT_SCHEDULER       1
#define PUTTYALT_TELNET_V2       1
#define PUTTYALT_PERF_TIMER      1
#define PUTTYALT_MEM_POOL        1
#define PUTTYALT_STR_BUF         1
#define PUTTYALT_INI_PARSER      1
#define PUTTYALT_TRIGGERS_V2     1
#define PUTTYALT_KEYGEN          1
#define PUTTYALT_HOST_VERIFY     1
#define PUTTYALT_IMG_PROTO       1
#define PUTTYALT_STATISTICS      1
#define PUTTYALT_PROXY_CHAIN     1
#define PUTTYALT_PALETTE         1
#define PUTTYALT_SCROLL_COMP     1
#define PUTTYALT_BASE64          1
#define PUTTYALT_ESC_PARSE       1
#define PUTTYALT_VALIDATION      1
#define PUTTYALT_SOUND           1

/* v1.0.0 — GUI, multiplexer, monitoring */
#define PUTTYALT_GUI             1
#define PUTTYALT_DIALOGS         1
#define PUTTYALT_TRAY            1
#define PUTTYALT_SCRIPT_MGR      1
#define PUTTYALT_CONN_PROF       1
#define PUTTYALT_MONITOR         1
#define PUTTYALT_MUX             1
#define PUTTYALT_SSH_AGENT       1
#define PUTTYALT_CLIP_MGR        1
#define PUTTYALT_LOG_ENGINE      1
#define PUTTYALT_CODE_SIGN       1
#define PUTTYALT_SESS_GROUP      1
#define PUTTYALT_RENDERER        1
#define PUTTYALT_SSH_CONFIG      1

/* v1.0.1 — Polish */
#define PUTTYALT_EXPECT          1
#define PUTTYALT_RECORDER        1
#define PUTTYALT_SEARCH_HL       1
#define PUTTYALT_NOTIF_CENTER    1
#define PUTTYALT_QUICK_LAUNCH    1
#define PUTTYALT_UNICODE_TABLE   1
#define PUTTYALT_DASHBOARD       1

/* v1.0.2 features */
#define PUTTYALT_HEXVIEW         1
#define PUTTYALT_FINGERPRINT     1
#define PUTTYALT_PORTFWD         1
#define PUTTYALT_COMPLETION      1
#define PUTTYALT_ACCESSIBILITY   1
#define PUTTYALT_RATE_LIMIT      1
#define PUTTYALT_CHATLOG         1
#define PUTTYALT_ENVMGR          1
#define PUTTYALT_SESSREPLAY      1
#define PUTTYALT_FILEBROWSER     1
#define PUTTYALT_INPUTFILTER     1
#define PUTTYALT_DEBUGCON        1
#define PUTTYALT_CLIPSYNC        1

/* v1.0.3 features */
#define PUTTYALT_THEMES          1
#define PUTTYALT_KEYBIND         1
#define PUTTYALT_SFTPMGR         1
#define PUTTYALT_BANDWIDTH       1
#define PUTTYALT_PROXY_CHAIN     1

/* v1.0.4 features */
#define PUTTYALT_SESSNOTES       1
#define PUTTYALT_PROTOANALYZE    1
#define PUTTYALT_HOTKEYMGR       1
#define PUTTYALT_COLORSCHEME     1
#define PUTTYALT_AUTOBACKUP      1
#define PUTTYALT_NETSCAN         1
#define PUTTYALT_TERMFX          1
#define PUTTYALT_LOCALE          1
#define PUTTYALT_TUNNELMON       1
#define PUTTYALT_CMDPALETTE      1
#define PUTTYALT_PERFMON         1

/* v1.0.6 features */
#define PUTTYALT_LAYOUT          1
#define PUTTYALT_MACRO           1
#define PUTTYALT_TERMINAL_CONF   1
#define PUTTYALT_UPDATER         1
#define PUTTYALT_STATUSBAR       1

#endif /* PUTTYALT_H */

/* v1.1.0 — Terminal Engine & UI Framework */
#define PUTTYALT_TERMEMU         1
#define PUTTYALT_SELECTION       1
#define PUTTYALT_RINGBUF         1
#define PUTTYALT_ANSIPARSE       1
#define PUTTYALT_COLORMGR        1
#define PUTTYALT_FONTCHAIN       1
#define PUTTYALT_URLDETECT_V2    1
#define PUTTYALT_CTXMENU         1
#define PUTTYALT_QUICKBAR        1
#define PUTTYALT_FAVORITES       1
#define PUTTYALT_HEALTHCHK       1
#define PUTTYALT_AUTORECONN      1
#define PUTTYALT_JUMPCHAIN       1
#define PUTTYALT_UILAYER         1
#define PUTTYALT_TOOLTIP         1
#define PUTTYALT_DOCKPANEL       1
#define PUTTYALT_CFGVALID        1
#define PUTTYALT_WIZARD          1
#define PUTTYALT_SESSREPLAY_V2   1
#define PUTTYALT_SCHEDIT         1
#define PUTTYALT_BELLCTL         1

/* v1.2.0 — UX & Usability */
#define PUTTYALT_SMARTPASTE      1
#define PUTTYALT_SESSSEARCH      1
#define PUTTYALT_PASSGEN         1
#define PUTTYALT_DRAGDROP        1
#define PUTTYALT_SNIPPETLIB      1
#define PUTTYALT_SESSNOTE_V2     1
#define PUTTYALT_PRESETS         1
#define PUTTYALT_NOTIFQUEUE      1
#define PUTTYALT_SEARCHOVERLAY   1
#define PUTTYALT_SSHKEYMGR       1
#define PUTTYALT_AUTOCOMPLETE_V2 1
#define PUTTYALT_CONNTIMEOUT     1
#define PUTTYALT_MULTICLIP       1
#define PUTTYALT_ENVVIEW         1
#define PUTTYALT_QUICKCMD        1

/* v1.3.0 — Workflow & Productivity */
#define PUTTYALT_SESSGROUP_V2    1
#define PUTTYALT_CMDHIST_V2      1
#define PUTTYALT_SFTPSYNC        1
#define PUTTYALT_OUTPUTCAP       1
#define PUTTYALT_CONNDASH        1
#define PUTTYALT_REGTRIG         1
#define PUTTYALT_FILEEDIT        1
#define PUTTYALT_CRONVIEW        1
#define PUTTYALT_SMARTRESIZE     1
#define PUTTYALT_TABCOLORS       1
#define PUTTYALT_SSHEXPORT       1

/* v1.3.1 — Patches */
#define PUTTYALT_OSDETECT        1
#define PUTTYALT_SPEEDTEST       1
#define PUTTYALT_TABPREVIEW      1
#define PUTTYALT_SESSDIFF        1
#define PUTTYALT_INPUTRECORD     1
#define PUTTYALT_LOCALECHO       1
#define PUTTYALT_RETRYPOLICY     1
#define PUTTYALT_TIMESTAMP       1
