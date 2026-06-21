#include "Thief.h"

#include <cmath>
#include <iostream>

namespace {
    float normalizeRouteDistance(float distance, float totalLength) {
        if (totalLength <= 0.f) {
            return 0.f;
        }

        float result = std::fmod(distance, totalLength);

        if (result < 0.f) {
            result += totalLength;
        }

        return result;
    }

    float clockwiseDistance(float from, float to, float totalLength) {
        float distance = to - from;

        if (distance < 0.f) {
            distance += totalLength;
        }

        return distance;
    }

    float forwardDistance(
        float from,
        float to,
        int direction,
        float totalLength
    ) {
        if (direction >= 0) {
            return clockwiseDistance(from, to, totalLength);
        }

        return clockwiseDistance(to, from, totalLength);
    }
}

Thief::Thief()
    : Character(24.f, sf::Color(230, 80, 80)),
      m_alertDistance(130.f),
      m_turnCooldown(1.5f),
      m_currentCooldown(0.f) {
    m_speed = 95.f;
    m_direction = 1;
}

void Thief::update(float dt) {
    moveAlongRoute(dt);

    if (m_currentCooldown > 0.f) {
        m_currentCooldown -= dt;

        if (m_currentCooldown < 0.f) {
            m_currentCooldown = 0.f;
        }
    }
}

bool Thief::checkTurnAround(float policeRouteDistance, float routeLength) {
    if (m_currentCooldown > 0.f || routeLength <= 0.f) {
        return false;
    }

    float thiefDistance = normalizeRouteDistance(
        getRouteDistance(),
        routeLength
    );

    float policeDistance = normalizeRouteDistance(
        policeRouteDistance,
        routeLength
    );

    float distanceInFront = forwardDistance(
        thiefDistance,
        policeDistance,
        getDirection(),
        routeLength
    );

    bool policeIsInFront = distanceInFront <= m_alertDistance;

    if (!policeIsInFront) {
        return false;
    }

    setDirection(-getDirection());
    m_currentCooldown = m_turnCooldown;

    std::cout << "[TEST] Thief turned around because police is in front."
              << std::endl;

    return true;
}

void Thief::setAlertDistance(float distance) {
    m_alertDistance = distance;
}

float Thief::getAlertDistance() const {
    return m_alertDistance;
}

float Thief::getDistanceTo(const sf::Vector2f& targetPosition) const {
    sf::Vector2f currentPosition = getPosition();

    float dx = currentPosition.x - targetPosition.x;
    float dy = currentPosition.y - targetPosition.y;

    return std::sqrt(dx * dx + dy * dy);
}

void Thief::draw(sf::RenderWindow& window) const {
    // 1. 身体
    window.draw(m_shape);

    sf::Vector2f center = m_shape.getPosition();
    float radius = m_shape.getRadius();

    // 2. 黑色眼罩底框
    sf::RectangleShape mask;
    mask.setSize({radius * 1.5f, radius * 0.45f});
    mask.setOrigin({radius * 1.5f / 2.f, radius * 0.45f / 2.f});
    mask.setPosition({
        center.x - radius * 0.10f,
        center.y - radius * 0.18f
    });
    mask.setFillColor(sf::Color::Black);
    window.draw(mask);

    // 3. 两个白色眼底
    float eyeRadius = radius * 0.17f;
    float pupilRadius = radius * 0.075f;

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

    window.draw(leftEye);
    window.draw(rightEye);

    // 4. 黑色瞳孔，仍然跟随移动方向偏移
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

    window.draw(leftPupil);
    window.draw(rightPupil);
}