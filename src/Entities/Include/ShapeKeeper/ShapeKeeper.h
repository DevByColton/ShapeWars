#ifndef SHAPEKEEPER_H
#define SHAPEKEEPER_H
#include "LasersBeams.h"
#include "ShapeKeeperBodyPart.h"
#include "ShapeKeeperCore.h"
#include "../../../Content/Include/Art.h"
#include "../../../GameState/Include/GamePlay.h"
#include "../../../GameState/UI/Include/GamePlayHUD.h"


struct ShapeKeeper {
    static ShapeKeeper &instance()
    {
        static auto *instance = new ShapeKeeper;
        return *instance;
    }

    ShapeKeeperCore core {
        Art::instance().shapeKeeperCore,
        {255, 51, 113, 255},
        &GamePlayHUD::instance().healthContainer.core
    };

    ShapeKeeperBodyPart top {
        Art::instance().shapeKeeperTop,
        Art::instance().shapeKeeperTopTrail,
        {255, 233, 38, 255},
        &GamePlayHUD::instance().healthContainer.top,
        &core,
        -8.f,
        -167.f
    };
    ShapeKeeperBodyPart middleLeft {
        Art::instance().shapeKeeperMiddleLeft,
        Art::instance().shapeKeeperMiddleLeftTrail,
        {53, 255, 203, 255},
        &GamePlayHUD::instance().healthContainer.middleLeft,
        &core,
        -167.f,
        -49.f
    };

    ShapeKeeperBodyPart middleRight {
        Art::instance().shapeKeeperMiddleRight,
        Art::instance().shapeKeeperMiddleRightTrail,
        {48, 255, 53, 255},
        &GamePlayHUD::instance().healthContainer.middleRight,
        &core,
        154.f,
        -49.f
    };

    ShapeKeeperBodyPart bottomLeft {
        Art::instance().shapeKeeperBottomLeft,
        Art::instance().shapeKeeperBottomLeftTrail,
        {255, 156, 25, 255},
        &GamePlayHUD::instance().healthContainer.bottomLeft,
        &core,
        -105.f,
        133.f
    };

    ShapeKeeperBodyPart bottomRight {
        Art::instance().shapeKeeperBottomRight,
        Art::instance().shapeKeeperBottomRightTrail,
        {241, 38, 255, 255},
        &GamePlayHUD::instance().healthContainer.bottomRight,
        &core,
        93.f,
        133.f
    };

    LasersBeams laserBeams {&core};
    GamePlayState* currentGamePlayState {nullptr};

    // Enemies spawn
    static constexpr float DEFAULT_TIME_UNTIL_ENEMIES_SPAWN = 5.f;
    float timeUntilEnemiesSpawnElapsed = DEFAULT_TIME_UNTIL_ENEMIES_SPAWN;
    float enemiesSpawningElapsed = 0.f;
    std::default_random_engine timeUntilEnemiesSpawnEngine {std::random_device{}()};
    std::default_random_engine enemiesSpawningTimeEngine {std::random_device{}()};
    std::uniform_real_distribution<float> timeUntilEnemiesSpawnDistribution {5.f, 15.f};
    std::uniform_real_distribution<float> enemiesSpawningTimeDistribution {5.f, 10.f};

    // Deactivate timing
    static constexpr float TIME_UNTIL_DEACTIVATE_DURATION = 3.5f;
    float timeUntilDeactivateElapsed = 0.f;

    bool isActive = false;
    bool isDefeated = false;

    void markDeactivate();
    void checkActivate();
    void checkDeactivate();
    void startEncounter();
    void endEncounter();
    bool canTakeCoreDamage() const;
    void updateEnemiesSpawn();
    void update();
    void draw();
};



#endif //SHAPEKEEPER_H
