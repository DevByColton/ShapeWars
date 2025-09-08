#ifndef ENEMIES_H
#define ENEMIES_H
#include <array>
#include <functional>
#include <random>
#include <numbers>
#include "../../Content/Include/Art.h"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/System/Vector2.hpp"


class Enemies {
private:
    class Enemy {
    private:
        union
        {
            struct
            {
                float xVelocity;
                float yVelocity;
                float speed;
                int pointValue;
                float timeUntilAct;
            };

            Enemy *next {nullptr};
        };

        const float maxTimeUntilAct = 1.0;
        bool shouldKill = false;
        std::function<void()> behavior;
        sf::Vector2f spriteSizeF {0.0, 0.0};
        sf::Sprite sprite {Art::instance().enemyPlaceholder};

        float halfWidth() const;
        float halfHeight() const;

    public:
        Enemy();

        float radius = 0;
        bool isActive = false;
        bool isActing = false;

        Enemy *getNext() const;
        void setNext(Enemy *);
        sf::Vector2f getPosition() const;
        void activateSeeker();
        void activateWanderer();
        void pushApartBy(const Enemy &);
        void reset();
        bool canAct();
        bool getShouldKill() const;
        void markForKill();
        void killAddPoints();
        void update();
        void draw() const;
    };

    static constexpr int MAX_ENEMY_COUNT = 200;
    Enemy *firstAvailable {nullptr};

    void spawnSeeker();
    void spawnWanderer();
    void resetEnemyPool();

public:
    Enemies();

    static Enemies &instance() {
        static auto *instance = new Enemies;
        return *instance;
    }

    std::array<Enemy, MAX_ENEMY_COUNT> enemies {};

    std::default_random_engine randEngine {std::random_device{}()};
    std::uniform_real_distribution<float> directionDistribution {0.0f, std::numbers::pi * 2 };
    float spawnChance = 60.0f;

    void killAll();
    void update();
    void draw() const;
};



#endif //ENEMIES_H
