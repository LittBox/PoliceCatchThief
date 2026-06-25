#include "Police.h"

#include <algorithm>

Police::Police()
    : Character(28.f, sf::Color(80, 160, 255)) {
    m_speed = 0.f;
    m_direction = 1;
}

//通过delta time，更新警察的状态，包括沿路径移动
void Police::update(float dt) {
    moveAlongRoute(dt);
}

//通过cpm(Characters Per Minute) 和accuracy，更新警察的移动速度
void Police::updateSpeedByTyping(float cpm, float accuracy) {

    const float factor = 0.4f;
    const float minSpeed = 0.f;
    const float maxSpeed = 260.f;

    float newSpeed =  cpm * accuracy * factor;

    //将新速度限制在最小和最大速度之间
    m_speed = std::clamp(newSpeed, minSpeed, maxSpeed);
}