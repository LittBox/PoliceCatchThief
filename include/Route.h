#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class Route {
public:
    Route();

    void draw(sf::RenderWindow& window) const;

    float getTotalLength() const;
    sf::Vector2f getPositionAt(float distance) const;

private:
    float distanceBetween(const sf::Vector2f& a, const sf::Vector2f& b) const;

private:
    std::vector<sf::Vector2f> m_points;
    float m_totalLength;
};