#include "map.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Shader.hpp"
#include "common.hpp"
#include "physics.hpp"
#include "tmxlite/Map.hpp"
#include "tmxlite/Object.hpp"
#include <algorithm>
#include <cmath>

const std::string PLATFORM_LAYER_NAME = "platforms";

const std::string fragment_shader = R"(
    void main() {
        gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0); 
    }
)";

struct Map::Impl {
    Physics::PhysicsManager &physics = *Physics::defaultProvider().get();
    tmx::Map map;
    std::vector<Physics::Body *> platforms{};
    std::vector<std::vector<sf::Vector2f>> platform_shapes{};
    sf::Vector2f player_position{0.0, 0.0};
    sf::RectangleShape map_rect;
    sf::Shader shader;

    Impl() = default;

    void loadTiledMap(const std::string &filename) {
        map.load(filename);
        bool found = false;

        for (const auto &layer : map.getLayers()) {
            auto name = layer->getName();
            if (name != PLATFORM_LAYER_NAME) {
                continue;
            }

            found = true;
            assert(layer->getType() == tmx::Layer::Type::Object);

            auto objects = layer->getLayerAs<tmx::ObjectGroup>().getObjects();
            for (const auto &object : objects) {
                if (object.getName() == "player") {
                    player_position = {object.getPosition().x, object.getPosition().y};
                } else {
                    auto points = object.getPoints();
                    sf::Vector2f position = {object.getPosition().x, object.getPosition().y};
                    std::vector<sf::Vector2f> sf_points{};
                    std::transform(points.begin(), points.end(), std::back_inserter(sf_points),
                                   [&](const auto &point) { return sf::Vector2f(point.x, point.y) + position; });
                    platform_shapes.push_back(sf_points);
                }
            }
        }

        assert(found);
    }

    void enter() {
        // assert(shader.loadFromMemory(fragment_shader, sf::Shader::Type::Fragment));
        // std::vector<sf::Vector2f> vertices;
        //
        // int size_x = std::ceil(map.getBounds().width);
        // int size_y = std::ceil(map.getBounds().height);
        // map_rect = sf::RectangleShape(sf::Vector2f(size_x, size_y));

        physics.reset();
        platforms.clear();
        for (auto &points : platform_shapes) {
            auto body = physics.createBody(Physics::BodyProperties{
                .bodyType = Physics::BodyType::STATIC,
                .position = {0.f, 0.f},
            });

            auto shape_props = Physics::ShapeProperties{};
            shape_props.categories = 1;
            shape_props.masks = 1;
            shape_props.group = 0;

            body->addChain(points, shape_props);
            platforms.push_back(body);
        }
    }

    void draw(sf::RenderTarget &target) const {
        // target.draw(map_rect, &shader);
    }
};

Map::Map() : _impl(std::make_unique<Impl>()) {}
Map::~Map() = default;
void Map::loadTiledMap(const std::string &filename) { _impl->loadTiledMap(filename); }
void Map::enter() { _impl->enter(); }
void Map::update(float dt) {}
void Map::draw(sf::RenderTarget &target) const {
    _impl->draw(target);
}
sf::Vector2f Map::playerPosition() const { return _impl->player_position; }
