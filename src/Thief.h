#pragma once

#include "Character.h"

class Thief : public Character {
public:
    Thief();

    void update(float dt) override;

    bool checkTurnAround(float policeRouteDistance, float routeLength);

    void setAlertDistance(float distance);
    float getAlertDistance() const;

private:
    float getDistanceTo(const sf::Vector2f& targetPosition) const;

private:
    float m_alertDistance;
    float m_turnCooldown;
    float m_currentCooldown;
};