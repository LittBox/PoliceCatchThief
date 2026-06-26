//封装角色共有属性
#pragma once

#include "GameObject.h"

#include <SFML/Graphics.hpp>    //提供图形绘制功能

//角色类，继承自GameObject，表示游戏中的角色对象
class Character : public GameObject {
public:
    //构造函数，初始化角色的形状、速度、方向和路线距离
    Character(float radius, sf::Color color);

    //实现GameObject的纯虚函数，更新角色状态
    void draw(sf::RenderWindow& window) const override;

    //沿路径移动
    void moveAlongRoute(float dt);

    //设置和获取角色的位置、速度、方向和路线距离
    void setPosition(const sf::Vector2f& position);
    sf::Vector2f getPosition() const;

    void setSpeed(float speed);
    float getSpeed() const;

    void setDirection(int direction);
    int getDirection() const;

    void setRouteDistance(float distance);
    float getRouteDistance() const;

    

protected:
    //角色的圆形形状
    sf::CircleShape m_shape;

    //角色的移动速度、方向、以及在路线上的距离
    float m_speed;
    int m_direction;
    float m_routeDistance;

    //角色面朝的方向向量
    sf::Vector2f m_facingDirection;
};