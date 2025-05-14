#pragma once

#include "arch.hpp"
#include <functional>
#include <unordered_map>

template <typename T> class StateMachine : public Component {
  private:
    std::unordered_map<T, std::function<void()>> state_enter;
    std::unordered_map<T, std::function<void(float)>> state_update;
    std::unordered_map<T, std::function<void()>> state_exit;

  public:
    T current_state;

    explicit StateMachine(T start) : current_state(start) {}

    void onEnter(T state, std::function<void()> func) { state_enter[state] = func; }

    void onUpdate(T state, std::function<void(float)> func) { state_update[state] = func; }

    void onExit(T state, std::function<void()> func) { state_exit[state] = func; }

    void moveTo(T newState) {
        auto it = state_exit.find(current_state);
        if (it != state_exit.end()) {
            it->second();
        }
        current_state = newState;
        it = state_enter.find(current_state);
        if (it != state_enter.end()) {
            it->second();
        }
    }

    void update(float dt) override {
        auto it = state_update.find(current_state);
        if (it != state_update.end()) {
            it->second(dt);
        }
    }

    ~StateMachine() override {
        auto it = state_exit.find(current_state);
        if (it != state_exit.end()) {
            it->second();
        }
    }
};
