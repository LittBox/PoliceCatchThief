#include "Thief.h"

#include <cmath>
#include <iostream>

//匿名命名空间，包含辅助函数，用于计算路线距离和方向
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

//实现Thief类的构造函数，初始化小偷的形状、速度、方向和路线距离
Thief::Thief()
    //初始化，自身属性，采用列表初始化
    : Character(24.f, sf::Color(230, 80, 80)), 
      m_alertDistance(130.f),
      m_turnCooldown(1.5f),
      m_currentCooldown(0.f) {
    //运行时设置小偷的初始状态，对象构造完后进行赋值
    m_speed = 95.f;
    m_direction = 1;
}

//按照每一帧更新小偷的状态，包括移动和掉头冷却时间的处理
void Thief::update(float dt) {
    moveAlongRoute(dt);

    if (m_currentCooldown > 0.f) {
        m_currentCooldown -= dt;

        if (m_currentCooldown < 0.f) {
            m_currentCooldown = 0.f;
        }
    }
}

//通过总路线长度和警察的路线距离，判断小偷是否需要掉头逃跑
//如果警察出现在小偷前方，并且距离小偷足够近，小偷就掉头逃跑；但如果小偷还在冷却时间内，就不能立刻再次掉头。
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

//绘制小偷的图形，包括身体、眼罩和眼睛，眼睛会根据小偷的移动方向偏移
void Thief::draw(sf::RenderWindow& window) const {
    //  身体
    window.draw(m_shape);

    sf::Vector2f center = m_shape.getPosition();
    float radius = m_shape.getRadius();

    //  黑色眼罩底框
    sf::RectangleShape mask;
    mask.setSize({radius * 1.5f, radius * 0.45f});
    mask.setOrigin({radius * 1.5f / 2.f, radius * 0.45f / 2.f});
    mask.setPosition({
        center.x - radius * 0.10f,
        center.y - radius * 0.18f
    });
    mask.setFillColor(sf::Color::Black);
    window.draw(mask);

    //  两个白色眼底
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

    //  黑色瞳孔，仍然跟随移动方向偏移
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