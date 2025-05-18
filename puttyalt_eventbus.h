#ifndef PUTTYALT_EVENTBUS_H
#define PUTTYALT_EVENTBUS_H

#define EVT_MAX_HANDLERS   64
#define EVT_MAX_TYPES      32
#define EVT_NAME_LEN       32

typedef enum {
    EVT_CONNECT = 0,
    EVT_DISCONNECT,
    EVT_DATA_RECEIVED,
    EVT_DATA_SENT,
    EVT_TAB_SWITCH,
    EVT_TAB_CLOSE,
    EVT_THEME_CHANGE,
    EVT_RESIZE,
    EVT_BELL,
    EVT_ERROR,
    EVT_CUSTOM = 16
} EventType;

typedef struct Event {
    EventType type;
    const void *data;
    int data_len;
    unsigned long timestamp;
} Event;

typedef void (*EventHandler)(const Event *evt, void *userdata);

typedef struct EventBinding {
    EventType type;
    EventHandler handler;
    void *userdata;
    int active;
} EventBinding;

typedef struct EventBus {
    EventBinding handlers[EVT_MAX_HANDLERS];
    int count;
    int dispatching;   /* re-entrancy guard */
} EventBus;

void evtbus_init(EventBus *bus);
int  evtbus_subscribe(EventBus *bus, EventType type,
                      EventHandler handler, void *userdata);
int  evtbus_unsubscribe(EventBus *bus, int handle);
void evtbus_publish(EventBus *bus, const Event *evt);
void evtbus_clear(EventBus *bus);

#endif
