#include "Police.h"

#include <algorithm>

Police::Police()
    : Character(28.f, sf::Color(80, 160, 255)) {
    m_speed = 0.f;
    m_direction = 1;
}

void Police::update(float dt) {
    moveAlongRoute(dt);
}

void Police::updateSpeedByTyping(float cpm, float accuracy) {

    const float factor = 0.4f;
    const float minSpeed = 0.f;
    const float maxSpeed = 260.f;

    float newSpeed =  cpm * accuracy * factor;

    m_speed = std::clamp(newSpeed, minSpeed, maxSpeed);
}