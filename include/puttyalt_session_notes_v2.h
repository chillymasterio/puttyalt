#ifndef PUTTYALT_SESSION_NOTES_V2_H
#define PUTTYALT_SESSION_NOTES_V2_H

typedef struct {
    int enabled;
    int initialized;
    void *data;
    int data_size;
} Session_notes_v2State;

int  session_notes_v2_init(Session_notes_v2State *s);
int  session_notes_v2_update(Session_notes_v2State *s);
void session_notes_v2_destroy(Session_notes_v2State *s);

#endif
