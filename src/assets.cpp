#include "assets.hpp"

namespace Assets {

AssetManager::~AssetManager() {}

class AssetManagerFS : public AssetManager {

  public:
    AssetManagerFS() = default;
    ~AssetManagerFS() override {};

    void registerTexture(const std::string &name, const std::string &path) override {
        assert(textures.find(name) == textures.end());

        sf::Texture texture;
        assert(texture.loadFromFile(path));
        textures[name] = texture;
    }

    void registerFont(const std::string &name, const std::string &path) override {
        assert(fonts.find(name) == fonts.end());

        sf::Font font;
        assert(font.openFromFile(path));
        font.setSmooth(false);

        fonts[name] = font;
    }

    sf::Texture &getTexture(const std::string &name) override {
        auto it = textures.find(name);
        if (it == textures.end()) {
            throw std::runtime_error("Texture not found: " + name);
        }
        return it->second;
    }

    sf::Font &getFont(const std::string &name) override {
        auto it = fonts.find(name);
        if (it == fonts.end()) {
            throw std::runtime_error("Font not found: " + name);
        }
        return it->second;
    }

  private:
    std::unordered_map<std::string, sf::Texture> textures;
    std::unordered_map<std::string, sf::Font> fonts;
};

std::shared_ptr<AssetManager> defaultProvider() {
    static std::shared_ptr<AssetManager> instance = std::make_shared<AssetManagerFS>();
    static bool initialized = false;

    if (!initialized) {
        instance->registerTexture("me", "assets/tex/me.png");
        instance->registerFont("default", "assets/fonts/monogram.ttf");
        initialized = true;
    }

    return instance;
}

}; // namespace Assets
