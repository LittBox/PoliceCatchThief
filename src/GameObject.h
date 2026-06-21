//所有游戏对象的抽象基类
#pragma once

#include <SFML/Graphics.hpp>

class GameObject {
public:
    virtual ~GameObject() = default;

    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderWindow& window) const = 0;
};