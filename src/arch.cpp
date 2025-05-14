#include "arch.hpp"

void GameObject::preUpdate(float dt) {}

void GameObject::update(float dt) {}

void GameObject::removeComponent(const std::string &name) {
    auto it = children.find(name);
    if (it != children.end()) {
        children.erase(it);
    }
}

void GameObject::updateAll(float dt) {
    preUpdate(dt);
    for (auto &[_, child] : children) {
        child->update(dt);
    }
    update(dt);
}
