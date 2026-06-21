#include "Character.h"
#include <cmath>
Character::Character(float radius, sf::Color color)
    : m_shape(radius),
      m_speed(0.f),
      m_direction(1),
      m_routeDistance(0.f),
      m_facingDirection(1.f, 0.f) {
    m_shape.setOrigin({radius, radius});
    m_shape.setFillColor(color);
}

void Character::draw(sf::RenderWindow& window) const {
    window.draw(m_shape);

    sf::Vector2f center = m_shape.getPosition();
    float radius = m_shape.getRadius();

    float eyeRadius = radius * 0.18f;
    float pupilRadius = radius * 0.08f;

    sf::Vector2f leftEyeCenter(
        center.x - radius * 0.32f,
        center.y - radius * 0.18f
    );

    sf::Vector2f rightEyeCenter(
        center.x + radius * 0.08f,
        center.y - radius * 0.18f
    );

    sf::CircleShape leftEye(eyeRadius);
    leftEye.setOrigin({eyeRadius, eyeRadius});
    leftEye.setPosition(leftEyeCenter);
    leftEye.setFillColor(sf::Color::White);

    sf::CircleShape rightEye(eyeRadius);
    rightEye.setOrigin({eyeRadius, eyeRadius});
    rightEye.setPosition(rightEyeCenter);
    rightEye.setFillColor(sf::Color::White);

    sf::Vector2f pupilOffset(
        m_facingDirection.x * eyeRadius * 0.35f,
        m_facingDirection.y * eyeRadius * 0.35f
    );

    sf::CircleShape leftPupil(pupilRadius);
    leftPupil.setOrigin({pupilRadius, pupilRadius});
    leftPupil.setPosition(leftEyeCenter + pupilOffset);
    leftPupil.setFillColor(sf::Color::Black);

    sf::CircleShape rightPupil(pupilRadius);
    rightPupil.setOrigin({pupilRadius, pupilRadius});
    rightPupil.setPosition(rightEyeCenter + pupilOffset);
    rightPupil.setFillColor(sf::Color::Black);

    window.draw(leftEye);
    window.draw(rightEye);
    window.draw(leftPupil);
    window.draw(rightPupil);
}

void Character::moveAlongRoute(float dt) {
    m_routeDistance += m_speed * static_cast<float>(m_direction) * dt;
}

void Character::setPosition(const sf::Vector2f& position) {
    sf::Vector2f oldPosition = m_shape.getPosition();
    sf::Vector2f delta = position - oldPosition;

    float length = std::sqrt(delta.x * delta.x + delta.y * delta.y);

    if (length > 0.001f) {
        m_facingDirection = {delta.x / length, delta.y / length};
    }

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

