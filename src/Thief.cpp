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