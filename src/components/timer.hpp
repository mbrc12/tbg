#pragma once
#include "arch.hpp"
#include <cmath>

class Timer: public Component {
public:
    explicit Timer(float loop_length = -1.f);
    ~Timer() override;

    void update(float dt) override;
    float elapsed();
    void restart();

private:
    float _elapsed = 0.f;
    float loop_length = -1.f;
};
