//封装角色共有属性
#pragma once

#include "GameObject.h"

#include <SFML/Graphics.hpp>

class Character : public GameObject {
public:
    Character(float radius, sf::Color color);

    void draw(sf::RenderWindow& window) const override;

    void moveAlongRoute(float dt);

    void setPosition(const sf::Vector2f& position);
    sf::Vector2f getPosition() const;

    void setSpeed(float speed);
    float getSpeed() const;

    void setDirection(int direction);
    int getDirection() const;

    void setRouteDistance(float distance);
    float getRouteDistance() const;

    

protected:
    sf::CircleShape m_shape;

    float m_speed;
    int m_direction;
    float m_routeDistance;

    sf::Vector2f m_facingDirection;
};