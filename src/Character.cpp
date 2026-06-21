#include "Character.h"

Character::Character(float radius, sf::Color color)
    : m_shape(radius),
      m_speed(0.f),
      m_direction(1),
      m_routeDistance(0.f) {
    m_shape.setOrigin({radius, radius});
    m_shape.setFillColor(color);
}

void Character::draw(sf::RenderWindow& window) const {
    window.draw(m_shape);
}

void Character::moveAlongRoute(float dt) {
    m_routeDistance += m_speed * static_cast<float>(m_direction) * dt;
}

void Character::setPosition(const sf::Vector2f& position) {
    m_shape.setPosition(position);
}

sf::Vector2f Character::getPosition() const {
    return m_shape.getPosition();
}

void Character::setSpeed(float speed) {
    m_speed = speed;
}

float Character::getSpeed() const {
    return m_speed;
}

void Character::setDirection(int direction) {
    if (direction >= 0) {
        m_direction = 1;
    } else {
        m_direction = -1;
    }
}

int Character::getDirection() const {
    return m_direction;
}

void Character::setRouteDistance(float distance) {
    m_routeDistance = distance;
}

float Character::getRouteDistance() const {
    return m_routeDistance;
}

