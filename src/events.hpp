#pragma once
#include "SFML/System/Vector2.hpp"
#include <functional>

namespace Events {

enum class EventKind {
    POSITION_CHANGED,
    TIMER_ELAPSED,
};

struct PositionChanged {
    sf::Vector2f new_position;
};

// Define associated types here

template<EventKind Kind> struct PayloadFor;

template <> struct PayloadFor<EventKind::POSITION_CHANGED> {
    typedef PositionChanged type; 
};

template <> struct PayloadFor<EventKind::TIMER_ELAPSED> {
    typedef void type;
};

typedef int EventBox;
typedef int CallbackId;

EventBox allocateBox();

extern EventBox last_event_box;
extern CallbackId last_callback_id;
extern std::unordered_map<EventBox, std::vector<std::pair<std::function<void(const void*)>, CallbackId>>> callbacks;

void reset();

template<EventKind Kind> void trigger(EventBox box, const typename PayloadFor<Kind>::type& payload) {
    auto it = callbacks.find(box);
    if (it != callbacks.end()) {
        for (auto& [callback, id] : it->second) {
            callback(static_cast<const void*>(&payload));
        }
    }
}

template<EventKind Kind> CallbackId subscribe(EventBox box, std::function<void(const typename PayloadFor<Kind>::type&)> callback) {
    auto it = callbacks.find(box);
    assert(it != callbacks.end());

    auto cb = [callback](const void* payload) {
        auto p = static_cast<const typename PayloadFor<Kind>::type*>(payload);
        callback(*p);
    }; 

    auto id = ++last_callback_id;
    it->second.push_back({cb, id});
}

void unsubscribe(EventBox box, CallbackId id);

};
