#include "events.hpp"
#include <algorithm>

namespace Events {

EventBox last_event_box = 0;
CallbackId last_callback_id = 0;
std::unordered_map<EventBox, std::vector<std::pair<std::function<void(const void*)>, CallbackId>>> callbacks = {};

void reset() {
    last_event_box = 0;
    last_callback_id = 0;
    callbacks.clear();
}

EventBox allocateBox() {
    last_event_box++;
    callbacks[last_event_box] = {};
    return last_event_box;
}

void unsubscribe(EventBox box, CallbackId id) {
    auto it = callbacks.find(box);
    assert(it != callbacks.end());
    auto& callbacks_list = it->second;

    callbacks_list.erase(std::find_if(callbacks_list.begin(), callbacks_list.end(), [id](const auto& pair) {
        return pair.second == id;
    }));
}

};
