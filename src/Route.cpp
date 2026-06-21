#include "Route.h"

#include <cmath>

Route::Route()
    : m_totalLength(0.f) {
    // 矩形闭环路线，位置控制在游戏区域内部，避开左上角信息卡片和底部输入区。
    m_points = {
        {160.f, 230.f},
        {780.f, 230.f},
        {780.f, 450.f},
        {160.f, 450.f}
    };

    for (std::size_t i = 0; i < m_points.size(); ++i) {
        const sf::Vector2f& current = m_points[i];
        const sf::Vector2f& next = m_points[(i + 1) % m_points.size()];
        m_totalLength += distanceBetween(current, next);
    }
}

void Route::draw(sf::RenderWindow& window) const {
    constexpr float thickness = 6.f;
    constexpr float pointRadius = 7.f;
    constexpr float PI = 3.14159265358979323846f;

    if (m_points.size() < 2) {
        return;
    }

    // 绘制路线线段
    for (std::size_t i = 0; i < m_points.size(); ++i) {
        const sf::Vector2f& start = m_points[i];
        const sf::Vector2f& end = m_points[(i + 1) % m_points.size()];

        const float dx = end.x - start.x;
        const float dy = end.y - start.y;
        const float length = std::sqrt(dx * dx + dy * dy);

        if (length <= 0.f) {
            continue;
        }

        const float angle = std::atan2(dy, dx) * 180.f / PI;

        sf::RectangleShape segment;
        segment.setSize({length, thickness});
        segment.setOrigin({0.f, thickness / 2.f});
        segment.setPosition(start);
        segment.setRotation(sf::degrees(angle));
        segment.setFillColor(sf::Color(150, 150, 160));

        window.draw(segment);
    }

    // 绘制路线节点，方便观察路线闭环
    for (const auto& point : m_points) {
        sf::CircleShape marker(pointRadius);
        marker.setOrigin({pointRadius, pointRadius});
        marker.setPosition(point);
        marker.setFillColor(sf::Color(210, 210, 220));

        window.draw(marker);
    }
}

float Route::getTotalLength() const {
    return m_totalLength;
}

sf::Vector2f Route::getPositionAt(float distance) const {
    if (m_points.empty() || m_totalLength <= 0.f) {
        return {0.f, 0.f};
    }

    // 让距离始终落在闭环路线总长度范围内
    float wrappedDistance = std::fmod(distance, m_totalLength);
    if (wrappedDistance < 0.f) {
        wrappedDistance += m_totalLength;
    }

    for (std::size_t i = 0; i < m_points.size(); ++i) {
        const sf::Vector2f& start = m_points[i];
        const sf::Vector2f& end = m_points[(i + 1) % m_points.size()];

        const float segmentLength = distanceBetween(start, end);

        if (wrappedDistance <= segmentLength) {
            const float ratio = wrappedDistance / segmentLength;

            return {
                start.x + (end.x - start.x) * ratio,
                start.y + (end.y - start.y) * ratio
            };
        }

        wrappedDistance -= segmentLength;
    }

    return m_points.front();
}

float Route::distanceBetween(const sf::Vector2f& a, const sf::Vector2f& b) const {
    const float dx = b.x - a.x;
    const float dy = b.y - a.y;
    return std::sqrt(dx * dx + dy * dy);
}