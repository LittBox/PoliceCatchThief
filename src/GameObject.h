//所有游戏对象的抽象基类
#pragma once

#include <SFML/Graphics.hpp>

class GameObject {
public:
    //虚析构函数，确保派生类对象被正确销毁
    virtual ~GameObject() = default;

    //纯虚函数，要求派生类实现更新逻辑
    virtual void update(float dt) = 0;
    
    //纯虚函数，要求派生类实现绘制逻辑
    virtual void draw(sf::RenderWindow& window) const = 0;
};