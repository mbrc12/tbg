#pragma once

#include "SFML/Graphics/RenderTarget.hpp"
#include <memory>
#include <string>

class Map {
  public:
    Map();
    ~Map();
    void loadTiledMap(const std::string &filename);
    void enter();
    void update(float dt);
    void draw(sf::RenderTarget &target) const;

    sf::Vector2f playerPosition() const;

  private:
    struct Impl;
    std::unique_ptr<Impl> _impl;
};
