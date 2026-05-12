#include "puttyalt_chatlog.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

void chatlog_init(ChatLog *cl)
{
    memset(cl, 0, sizeof(*cl));
    cl->filter_type = -1;
}

int chatlog_add(ChatLog *cl, ChatMsgType type, const char *user, const char *text)
{
    if (cl->paused) return -1;
    ChatMsg *m = &cl->messages[cl->write_pos];
    memset(m, 0, sizeof(*m));
    m->type = type;
    if (user) snprintf(m->user, CHAT_MAX_USER, "%s", user);
    if (text) snprintf(m->text, CHAT_MAX_TEXT, "%s", text);
    m->timestamp = (long)time(NULL);

    cl->write_pos = (cl->write_pos + 1) % CHAT_MAX_MSGS;
    if (cl->count < CHAT_MAX_MSGS) cl->count++;
    return 0;
}

int chatlog_add_marker(ChatLog *cl, const char *label)
{
    return chatlog_add(cl, CHAT_MSG_MARKER, NULL, label);
}

void chatlog_clear(ChatLog *cl)
{
    cl->count = 0;
    cl->write_pos = 0;
}

int chatlog_search(const ChatLog *cl, const char *query, int start)
{
    if (!query || !*query) return -1;
    for (int i = start; i < cl->count; i++) {
        if (strstr(cl->messages[i].text, query))
            return i;
    }
    return -1;
}

int chatlog_export(const ChatLog *cl, const char *path, int html)
{
    FILE *f = fopen(path, "w");
    if (!f) return -1;

    if (html) {
        fprintf(f, "<!DOCTYPE html><html><head><meta charset=\"utf-8\">"
                   "<title>Session Log</title>"
                   "<style>body{font-family:monospace;background:#1B2838;color:#C8D6E5;padding:20px;}"
                   ".input{color:#4A9EE0}.output{color:#C8D6E5}"
                   ".error{color:#E74C3C}.system{color:#888;font-style:italic}"
                   ".marker{color:#F39C12;border-top:1px solid #333;padding-top:8px}"
                   ".time{color:#555;font-size:0.8em}</style></head><body>\n");
    }

    for (int i = 0; i < cl->count; i++) {
        const ChatMsg *m = &cl->messages[i];
        if (html) {
            const char *cls = "output";
            switch (m->type) {
                case CHAT_MSG_INPUT:  cls = "input"; break;
                case CHAT_MSG_ERROR:  cls = "error"; break;
                case CHAT_MSG_SYSTEM: cls = "system"; break;
                case CHAT_MSG_MARKER: cls = "marker"; break;
                default: break;
            }
            fprintf(f, "<div class=\"%s\">", cls);
            if (m->user[0]) fprintf(f, "<b>%s</b>: ", m->user);
            fprintf(f, "%s</div>\n", m->text);
        } else {
            const char *prefix = ">";
            switch (m->type) {
                case CHAT_MSG_INPUT:  prefix = ">>>"; break;
                case CHAT_MSG_ERROR:  prefix = "ERR"; break;
                case CHAT_MSG_SYSTEM: prefix = "SYS"; break;
                case CHAT_MSG_MARKER: prefix = "---"; break;
                default: break;
            }
            fprintf(f, "[%s] %s %s\n", prefix,
                    m->user[0] ? m->user : "-", m->text);
        }
    }

    if (html) fprintf(f, "</body></html>\n");
    fclose(f);
    return 0;
}

void chatlog_set_filter(ChatLog *cl, int type, const char *text)
{
    cl->filter_type = type;
    if (text)
        snprintf(cl->filter_text, sizeof(cl->filter_text), "%s", text);
    else
        cl->filter_text[0] = '\0';
}

int chatlog_get_filtered(const ChatLog *cl, int *indices, int max_indices)
{
    int n = 0;
    for (int i = 0; i < cl->count && n < max_indices; i++) {
        const ChatMsg *m = &cl->messages[i];
        if (cl->filter_type >= 0 && m->type != cl->filter_type)
            continue;
        if (cl->filter_text[0] && !strstr(m->text, cl->filter_text))
            continue;
        indices[n++] = i;
    }
    return n;
}

void chatlog_pause(ChatLog *cl, int pause)
{
    cl->paused = pause;
}

void chatlog_mark(ChatLog *cl, int index)
{
    if (index >= 0 && index < cl->count)
        cl->messages[index].marked = !cl->messages[index].marked;
}
