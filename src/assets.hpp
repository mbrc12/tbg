#pragma once

#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/Texture.hpp"
#include <string>
namespace Assets {

class AssetManager {
public:
    virtual ~AssetManager() = 0;

    virtual void registerTexture(const std::string &name, const std::string &filename) = 0;
    virtual void registerFont(const std::string &name, const std::string &filename) = 0;

    virtual sf::Texture& getTexture(const std::string &name) = 0;
    virtual sf::Font& getFont(const std::string &name) = 0;
};

std::shared_ptr<AssetManager> defaultProvider();

};
