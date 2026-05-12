#ifndef PUTTYALT_CHATLOG_H
#define PUTTYALT_CHATLOG_H

#define CHAT_MAX_MSGS    4096
#define CHAT_MAX_TEXT     1024
#define CHAT_MAX_USER     64

typedef enum {
    CHAT_MSG_INPUT = 0,
    CHAT_MSG_OUTPUT,
    CHAT_MSG_SYSTEM,
    CHAT_MSG_ERROR,
    CHAT_MSG_MARKER
} ChatMsgType;

typedef struct {
    ChatMsgType type;
    char        user[CHAT_MAX_USER];
    char        text[CHAT_MAX_TEXT];
    long        timestamp;
    int         marked;
} ChatMsg;

typedef struct {
    ChatMsg  messages[CHAT_MAX_MSGS];
    int      count;
    int      write_pos;
    int      filter_type;
    char     filter_text[128];
    int      paused;
} ChatLog;

void chatlog_init(ChatLog *cl);
int  chatlog_add(ChatLog *cl, ChatMsgType type, const char *user, const char *text);
int  chatlog_add_marker(ChatLog *cl, const char *label);
void chatlog_clear(ChatLog *cl);
int  chatlog_search(const ChatLog *cl, const char *query, int start);
int  chatlog_export(const ChatLog *cl, const char *path, int html);
void chatlog_set_filter(ChatLog *cl, int type, const char *text);
int  chatlog_get_filtered(const ChatLog *cl, int *indices, int max_indices);
void chatlog_pause(ChatLog *cl, int pause);
void chatlog_mark(ChatLog *cl, int index);

#endif
