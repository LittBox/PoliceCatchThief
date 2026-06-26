#pragma once

#include "Character.h"

//警察类，继承自Character，表示游戏中的警察角色
class Police : public Character {
public:
    //构造函数，初始化警察的形状、速度、方向和路线距离
    Police();

    //实现Character的纯虚函数，更新警察状态
    void update(float dt) override;

    //根据打字速度和准确率更新警察的移动速度
    void updateSpeedByTyping(float cpm, float accuracy);
};