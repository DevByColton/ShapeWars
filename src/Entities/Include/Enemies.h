#ifndef ENEMIES_H
#define ENEMIES_H
#include <array>
#include <functional>
#include <random>
#include <numbers>
#include "../../GameRoot.h"
#include "../../Content/Art.h"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/System/Vector2.hpp"


class Enemies {
private:
    struct EnemyKillText {
        EnemyKillText() = default;
        ~EnemyKillText() = default;

        const float baseFadeTime = 1.0f;
        float fadeTime = 0;
        const float baseFadeScale = 1.0f;
        float fadeScale = 0;
        sf::Text text {Art::instance().font, {}, 16};
        sf::Vector2f position {0.0, 0.0};
        bool isActive = false;

        void fadeIn();
        void reset();
    };

    std::array<EnemyKillText, 200> enemyKillTexts {};

    int nextKillTextIndex = 0;
    int nextEnemyIndex = 0;
    void incrementNextEnemyIndex();
    void addKillText(int, sf::Vector2f);
    sf::Vector2f createSpawnPosition();

public:
    Enemies() = default;
    ~Enemies() = default;

    static Enemies &instance() {
        static auto *instance = new Enemies;
        return *instance;
    }

    struct Enemy {
        Enemy();
        ~Enemy() = default;

        sf::Sprite sprite {Art::instance().enemyPlaceholder};
        sf::Vector2f spriteSizeF {0.0, 0.0};
        sf::Vector2f velocity {0.0, 0.0};
        float speed = 0;
        float radius = 0;
        bool isActive = false;
        const float maxTimeUntilAct = 1;
        float timeUntilAct = maxTimeUntilAct;
        int pointValue = 0;
        std::function<void()> behavior;

        sf::Vector2f getPosition() const;
        float halfWidth() const;
        float halfHeight() const;
        void pushApartBy(const Enemy &);
        void reset();
        bool canAct();
        void killAddPoint();
        void killNoPoints();

    };

    std::array<Enemy, 200> enemies {};
    std::default_random_engine randEngine {std::random_device{}()};
    std::uniform_real_distribution<float> directionDistribution {0.0f, std::numbers::pi * 2 };
    std::uniform_real_distribution<float> xPositionDistribution {0.0f, GameRoot::instance().windowSizeF.x};
    std::uniform_real_distribution<float> yPositionDistribution {0.0f, GameRoot::instance().windowSizeF.y};
    float spawnChance = 60.0f;

    void killAll();
    void spawnSeeker();
    void spawnWanderer();
    void update();
    void draw();
};



#endif //ENEMIES_H
