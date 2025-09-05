#ifndef SPAWNHELPER_H
#define SPAWNHELPER_H
#include <random>
#include "../../GameRoot.h"


struct SpawnHelper {
    SpawnHelper() = default;
    ~SpawnHelper() = default;

    static SpawnHelper &instance()
    {
        static auto *instance = new SpawnHelper;
        return *instance;
    }

    std::default_random_engine randEngine {std::random_device{}()};
    std::uniform_real_distribution<float> xPositionDistribution {0.0f, GameRoot::instance().windowSizeF.x};
    std::uniform_real_distribution<float> yPositionDistribution {0.0f, GameRoot::instance().windowSizeF.y};
    float positionOffset = 250.0;

    sf::Vector2f createSpawnPosition();
};



#endif //SPAWNHELPER_H
