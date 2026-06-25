#pragma once

#include "Character.h"

class Thief : public Character {
public:
    //构造函数
    Thief();

    //实现Character的纯虚函数，更新小偷状态
    void update(float dt) override;

    //实现Character的纯虚函数，绘制小偷
    void draw(sf::RenderWindow& window) const override;

    //检查小偷是否需要掉头，基于警察的路线距离和总路线长度
    bool checkTurnAround(float policeRouteDistance, float routeLength);

    //设置和获取小偷的警戒距离
    void setAlertDistance(float distance);
    float getAlertDistance() const;

private:
    //计算小偷与目标位置的距离
    float getDistanceTo(const sf::Vector2f& targetPosition) const;

private:
    //私有成员变量，表示小偷的警戒距离、掉头冷却时间和当前冷却时间
    float m_alertDistance;
    float m_turnCooldown;
    float m_currentCooldown;
};