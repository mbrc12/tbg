#include "timer.hpp"

Timer::Timer(float loop_length) : loop_length{loop_length}, _elapsed{0.f} {}

void Timer::update(float dt) {
    _elapsed += dt;
    if (loop_length > 0.f && _elapsed >= loop_length) {
        _elapsed = std::fmod(_elapsed, loop_length);
    }
}

void Timer::restart() {
    _elapsed = 0.f;
}

float Timer::elapsed() {
    return _elapsed;
}

Timer::~Timer() = default;
