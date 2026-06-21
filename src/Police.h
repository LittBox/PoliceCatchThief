#pragma once

#include "Character.h"

class Police : public Character {
public:
    Police();

    void update(float dt) override;

    void updateSpeedByTyping(float cpm, float accuracy);
};