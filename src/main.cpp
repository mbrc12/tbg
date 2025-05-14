#include "colors.hpp"
#include "input.hpp"
#include "common.hpp"
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cmath>

#include <imgui-SFML.h>
#include <imgui.h>
#include <iostream>

namespace ImSFML = ImGui::SFML;
namespace Im = ImGui;

constexpr int WIDTH = 1280;
constexpr int HEIGHT = 720;

constexpr float IDEAL_RATIO = static_cast<float>(WIDTH) / HEIGHT;
constexpr int FPS = 60;
constexpr auto PER_FRAME = sf::milliseconds(1000 / FPS);
constexpr int FPS_HISTORY = 100;

void resizer(sf::RenderWindow &window);

int main() {
    // auto modes = sf::VideoMode::getFullscreenModes();
    // if (modes.empty()) {
    //     std::cerr << "No fullscreen modes available." << std::endl;
    //     return 1;
    // }
    // auto mode = modes[0];
    if (!sf::Shader::isAvailable()) {
        std::cerr << "Shaders are not available." << std::endl;
        return 1;
    }

    auto mode = sf::VideoMode({WIDTH, HEIGHT});

    sf::RenderWindow window(mode, "Game", sf::Style::Default, sf::State::Windowed);
    assert(ImSFML::Init(window));

    DBG(window.getSettings().majorVersion << " " << window.getSettings().minorVersion);

    sf::RenderTexture screen({WIDTH, HEIGHT});
    screen.setSmooth(false);

    window.setVerticalSyncEnabled(true);

    sf::Clock clock;

    // physics.build();

    // Draw cursor using imgui
    Im::SetMouseCursor(ImGuiMouseCursor_TextInput);
    auto &io = Im::GetIO();
    io.MouseDrawCursor = true;

    auto &input = *Input::defaultProvider().get();

    sf::Font font("assets/fonts/monogram.ttf");
    font.setSmooth(false);

    sf::Text status_text(font);
    status_text.setCharacterSize(32);
    status_text.setFillColor(THEME_WHITE);
    status_text.setPosition({0, -16});

    float current_fps = 0;

    std::array<float, 100> fps_history = {0};

    sf::CircleShape circle(10.f);
    circle.setFillColor(THEME_WHITE);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            ImSFML::ProcessEvent(window, *event);

            if (event->is<sf::Event::Closed>()) {
                goto end;
            }
            if (event->is<sf::Event::Resized>()) {
                resizer(window);
            }
        }

        // fps calculations
        auto dt = clock.restart();
        float dt_s = dt.asSeconds();

        int instantaneous_fps = 1.f / dt_s;
        std::shift_left(fps_history.begin(), fps_history.end(), 1);
        fps_history[FPS_HISTORY - 1] = instantaneous_fps;

        current_fps = current_fps * 0.9 + instantaneous_fps * 0.1;

        ImSFML::Update(window, dt);

        auto fps_str = std::format("fps: {:.1f}", current_fps);

        Im::Begin("Game", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        Im::PlotLines(fps_str.c_str(), fps_history.data(), FPS_HISTORY, 0, 0, 55, 65);

        screen.clear(THEME_BLACK);

        using Input::Action;

        input.update();
        if (input.isPressed(Action::EXIT)) {
            goto end;
        }

        auto mouse_pos = io.MousePos;

        Im::Text("buttons: %s", input.status().c_str());
        Im::Text("mouse: %.2f, %.2f", mouse_pos.x, mouse_pos.y);

        circle.setPosition(circle.getPosition() + input.normalizedDirection() * 500.f * dt_s);
        screen.draw(circle);
        screen.display();

        window.clear(sf::Color::Black);
        window.draw(sf::Sprite(screen.getTexture()));
        Im::End();
        ImSFML::Render(window);
        window.display();
    }

end:
    window.close();
    return 0;
}

void resizer(sf::RenderWindow &window) {
    auto [width, height] = window.getSize();
    float ratio = static_cast<float>(width) / height;

    sf::View view(sf::FloatRect({0.f, 0.f}, {WIDTH, HEIGHT}));

    sf::FloatRect viewport;

    if (ratio > IDEAL_RATIO) {
        viewport = sf::FloatRect({(1 - IDEAL_RATIO / ratio) / 2.f, 0.f}, {IDEAL_RATIO / ratio, 1.f});
    } else {
        float h = ratio / IDEAL_RATIO;
        viewport = sf::FloatRect({0.f, (1 - h) / 2.f}, {1.f, h});
    }

    view.setViewport(viewport);
    window.setView(view);
}
