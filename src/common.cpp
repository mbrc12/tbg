#include "common.hpp"

std::string str(const sf::FloatRect &rect) {
    return std::format("(nw: {:.1f}, {:.1f}; sz: {:.1f} x {:.1f})", rect.position.x, rect.position.y, rect.size.x,
                       rect.size.y);
}

std::ostream &operator<<(std::ostream &os, const sf::FloatRect &rect) {
    os << str(rect);
    return os;
}
