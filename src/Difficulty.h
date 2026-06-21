#pragma once

#include <string>

struct DifficultyConfig {
    std::string name;
    float timeLimit;
    float thiefSpeed;
    float alertDistance;
    float captureDistance;
    std::string wordFile;
};