#include "input.hpp"
#include "common.hpp"
#include <SFML/Window/Joystick.hpp>

namespace Input {

struct InputManager::Impl {
    using ActionState = map<Action, bool>;

    int joystick_id = -1;
    InputConfig config;
    ActionState last_state;
    ActionState current_state;
    ActionState just_pressed;
    ActionState just_released;

    Impl(InputConfig config) : config(config) {
        last_state = emptyState();
        current_state = emptyState();
        just_pressed = emptyState();
        just_released = emptyState();

        for (int i = 0; i < sf::Joystick::Count; i++) {
            if (sf::Joystick::isConnected(i)) {
                joystick_id = i;
                auto joystick = sf::Joystick::getIdentification(joystick_id).name.toAnsiString();
                cout << "Joystick detected:  " << joystick << endl;
                break;
            }
        }
    }

    ActionState emptyState() {
        map<Action, bool> state;
        for (const auto &action : config) {
            state[action.first] = false;
        }
        return state;
    }

    map<JoystickButton, bool> joystickState() {
        using namespace sf::Joystick;

        if (joystick_id < 0) {
            return {};
        }

        auto button_count = getButtonCount(joystick_id);

        auto button = [&](int button) { return button < button_count && isButtonPressed(this->joystick_id, button); };

        auto axis = [&](Axis ax, bool above, float threshold) {
            return hasAxis(this->joystick_id, ax) && ((above && getAxisPosition(joystick_id, ax) > threshold) ||
                                                     (!above && getAxisPosition(joystick_id, ax) < threshold));
        };

        map<JoystickButton, bool> state;
        state[JoystickButton::A] = button(0);
        state[JoystickButton::B] = button(1);
        state[JoystickButton::X] = button(2);
        state[JoystickButton::Y] = button(3);
        state[JoystickButton::LEFT_BUMPER] = button(4);
        state[JoystickButton::RIGHT_BUMPER] = button(5);

        static bool ABOVE = true;
        static bool BELOW = false;

        state[JoystickButton::LEFT] = axis(Axis::X, BELOW, -50) || axis(Axis::PovX, BELOW, -50);
        state[JoystickButton::RIGHT] = axis(Axis::X, ABOVE, 50) || axis(Axis::PovX, ABOVE, 50);
        state[JoystickButton::UP] = axis(Axis::Y, BELOW, -50) || axis(Axis::PovY, BELOW, -50);
        state[JoystickButton::DOWN] = axis(Axis::Y, ABOVE, 50) || axis(Axis::PovY, ABOVE, 50);
        state[JoystickButton::LEFT_TRIGGER] = axis(Axis::Z, ABOVE, 50);
        state[JoystickButton::RIGHT_TRIGGER] = axis(Axis::R, ABOVE, 50);

        return state;
    }

    void update() {
        last_state = current_state;
        current_state = emptyState();
        just_pressed = emptyState();
        just_released = emptyState();

        const auto joystick = joystickState();

        for (const auto &[action_name, action_config] : config) {

            bool is_pressed = false;

            for (const auto &key : action_config.keyboard) {
                if (sf::Keyboard::isKeyPressed(key)) {
                    is_pressed = true;
                    break;
                }
            }

            if (!is_pressed && !joystick.empty()) {
                for (const auto &button : action_config.joystick) {
                    if (joystick.find(button) != joystick.end() && joystick.at(button)) {
                        is_pressed = true;
                        break;
                    }
                }
            }

            current_state[action_name] = is_pressed;

            if (is_pressed && !last_state[action_name]) {
                just_pressed[action_name] = true;
            } else if (!is_pressed && last_state[action_name]) {
                just_released[action_name] = true;
            }
        }
    }
};

InputManager::InputManager(InputConfig config) : _impl(std::make_unique<Impl>(config)) {}
InputManager::~InputManager() = default;

void InputManager::update() { _impl->update(); }

bool InputManager::isPressed(Action action) const { return _impl->current_state.at(action); }

bool InputManager::isJustPressed(Action action) const { return _impl->just_pressed.at(action); }

bool InputManager::isJustReleased(Action action) const { return _impl->just_released.at(action); }

sf::Vector2f InputManager::direction() const {
    sf::Vector2f dir(0, 0);
    if (isPressed(Action::LEFT)) {
        dir.x -= 1;
    }
    if (isPressed(Action::RIGHT)) {
        dir.x += 1;
    }
    if (isPressed(Action::UP)) {
        dir.y -= 1;
    }
    if (isPressed(Action::DOWN)) {
        dir.y += 1;
    }
    return dir;
}

sf::Vector2f InputManager::normalizedDirection() const {
    auto dir = direction();
    if (dir.lengthSquared() > 1e-6) {
        return dir.normalized();
    }
    return dir;
}

std::string InputManager::status() const {
    std::stringstream status;
    for (const auto &[action, repr] : ACTION_REPRESENTATIONS) {
        if (isPressed(action)) {
            status << repr;
        } else {
            status << "_";
        }
    }
    return status.str();
}

std::shared_ptr<InputManager> defaultProvider() {
    static std::shared_ptr<InputManager> instance = std::make_shared<InputManager>(DEFAULT_CONFIG);
    return instance;
}

} // namespace Input
//
//
// Code to print joystick state ////////////////////////
// std::stringstream ss;
//
// for (auto& axis : {
//     sf::Joystick::Axis::X,
//     sf::Joystick::Axis::Y,
//     sf::Joystick::Axis::Z,
//     sf::Joystick::Axis::R,
//     sf::Joystick::Axis::U,
//     sf::Joystick::Axis::V,
//     sf::Joystick::Axis::PovX,
//     sf::Joystick::Axis::PovY
// }) {
//     if (joystickId >= 0 && sf::Joystick::hasAxis(joystickId, axis)) {
//         auto value = sf::Joystick::getAxisPosition(joystickId, axis);
//         ss << value << ",";
//     }
// }
//
// ss << "|";
//
// for (unsigned int button = 0; button <
// sf::Joystick::getButtonCount(joystickId); ++button) {
//     if (joystickId >= 0 && sf::Joystick::isButtonPressed(joystickId, button))
//     {
//         ss << button%10;
//     } else {
//         ss << "_";
//     }
// }
//
// cout << ss.str() << endl;
//
