#pragma once
#include <map>
#include <memory>
#include <cassert>

class Component {
  public:
    virtual void update(float dt) = 0;
    virtual ~Component() = default;
};

class GameObject {
  protected:
    virtual void preUpdate(float dt);
    virtual void update(float dt);

  public:
    void updateAll(float dt);
    virtual ~GameObject() = default;

    template <typename T> std::shared_ptr<T> addComponent(const std::string &name, T component) {
        auto ptr = std::make_shared<T>(component);
        children[name] = ptr;
        return ptr;
    }

    template <typename T> std::shared_ptr<T> getComponent(const std::string &name) {
        auto it = children.find(name);
        if (it != children.end()) {
            auto ptr = std::dynamic_pointer_cast<T>(it->second);
            assert(ptr != nullptr);
            return ptr;
        }
        return nullptr;
    }

    void removeComponent(const std::string &name);

  private:
    std::map<std::string, std::shared_ptr<Component>> children;
};
