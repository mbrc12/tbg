#pragma once
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>
#include <format>
#include <iostream>
#define HERE std::cout << __FILE__ << ":" << __LINE__ << std::endl;
#define DBG(x) std::cout << __FILE__ << ":" << __LINE__ << ": " << x << std::endl;

using std::cout, std::endl;

constexpr float EPS = 1e-6f;

// Templated functions need to be in the header file
template <typename T> std::string str(const sf::Vector2<T> &vec) {
    if constexpr (std::is_same_v<T, int>) {
        return std::format("({}, {})", vec.x, vec.y);
    } else {
        return std::format("({:.1f}, {:.1f})", static_cast<float>(vec.x), static_cast<float>(vec.y));
    }
}

// This function is not templated, so we define it in the CPP file to prevent ODR.
std::string str(const sf::FloatRect &rect);


template <typename T> std::ostream &operator<<(std::ostream &os, const sf::Vector2<T> &vec) {
    os << str(vec);
    return os;
}

std::ostream &operator<<(std::ostream &os, const sf::FloatRect &rect);

template<typename T> std::ostream &operator<<(std::ostream &os, const std::vector<T> &v) {
    os << "[";
    for (const auto& it: v) {
        os << it << ", ";
    }
    os << "]";
    return os;
}
