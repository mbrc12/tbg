#pragma once

#include <SFML/Window.hpp>
#include <string>
#include <unordered_map>
#include <vector>

namespace Input {

enum class Action {
    LEFT,
    RIGHT,
    UP,
    DOWN,
    JUMP,
    DASH,
    EXIT,
};

const std::vector<std::pair<Action, std::string>> ACTION_REPRESENTATIONS = {
    // order matters
    {Action::LEFT, "L"}, {Action::RIGHT, "R"}, {Action::UP, "U"},
    {Action::DOWN, "D"}, {Action::JUMP, "~"},  {Action::DASH, "!"},
};

enum class JoystickButton {
    A,
    B,
    X,
    Y,
    LEFT,
    RIGHT,
    UP,
    DOWN,
    LEFT_BUMPER,
    RIGHT_BUMPER,
    LEFT_TRIGGER,
    RIGHT_TRIGGER,
};

template <typename K, typename V> using map = std::unordered_map<K, V>;

struct ActionConfig {
    std::vector<sf::Keyboard::Key> keyboard;
    std::vector<JoystickButton> joystick;
};

using InputConfig = map<Action, ActionConfig>;

using sf::Keyboard::Key;
const InputConfig DEFAULT_CONFIG = {
    {Action::LEFT, {{Key::A, Key::Left}, {JoystickButton::LEFT}}},
    {Action::RIGHT, {{Key::D, Key::Right}, {JoystickButton::RIGHT}}},
    {Action::UP, {{Key::W, Key::Up}, {JoystickButton::UP}}},
    {Action::DOWN, {{Key::S, Key::Down}, {JoystickButton::DOWN}}},
    {Action::JUMP, {{Key::Space}, {JoystickButton::A, JoystickButton::LEFT_BUMPER}}},
    {Action::DASH, {{Key::RBracket}, {JoystickButton::RIGHT_TRIGGER}}},
    {Action::EXIT, {{Key::Escape}, {}}},
};

class InputManager {
  public:
    explicit InputManager(InputConfig config);
    ~InputManager();

    void update();
    bool isPressed(Action action) const;
    bool isJustPressed(Action action) const;
    bool isJustReleased(Action action) const;
    sf::Vector2f direction() const;
    sf::Vector2f normalizedDirection() const;

    std::string status() const;

  private:
    class Impl;
    std::unique_ptr<Impl> _impl;
};

std::shared_ptr<InputManager> defaultProvider();

} // namespace Input
