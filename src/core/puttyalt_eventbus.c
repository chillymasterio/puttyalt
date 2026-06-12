#include <string.h>
#include "puttyalt_eventbus.h"

void evtbus_init(EventBus *bus)
{
    memset(bus, 0, sizeof(*bus));
}

int evtbus_subscribe(EventBus *bus, EventType type,
                     EventHandler handler, void *userdata)
{
    if (bus->count >= EVT_MAX_HANDLERS) return -1;
    if (!handler) return -1;

    EventBinding *b = &bus->handlers[bus->count];
    b->type = type;
    b->handler = handler;
    b->userdata = userdata;
    b->active = 1;
    return bus->count++;
}

int evtbus_unsubscribe(EventBus *bus, int handle)
{
    if (handle < 0 || handle >= bus->count) return -1;
    bus->handlers[handle].active = 0;
    return 0;
}

void evtbus_publish(EventBus *bus, const Event *evt)
{
    if (bus->dispatching) return;
    /* prevent re-entrancy */
    bus->dispatching = 1;

    for (int i = 0; i < bus->count; i++) {
        EventBinding *b = &bus->handlers[i];
        if (b->active && b->type == evt->type)
            b->handler(evt, b->userdata);
    }

    bus->dispatching = 0;
}

void evtbus_clear(EventBus *bus)
{
    for (int i = 0; i < bus->count; i++)
        bus->handlers[i].active = 0;
    bus->count = 0;
}
