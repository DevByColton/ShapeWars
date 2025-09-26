#ifndef ENEMIES_H
#define ENEMIES_H
#include <array>
#include <functional>
#include <random>
#include "../../Content/Include/Art.h"
#include "../../System/Include/Extensions.h"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/System/Clock.hpp"
#include "SFML/System/Time.hpp"
#include "SFML/System/Vector2.hpp"


enum EnemyType
{
    None,
    Dodger,
    Seeker,
    Wanderer
};


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
        const float halfMaxTimeUntilAct = maxTimeUntilAct / 2.f;
        std::function<void()> behavior;
        sf::Vector2f spriteSizeF {0.0, 0.0};
        sf::Sprite sprite {Art::instance().enemyPlaceholder};

        float halfWidth() const;
        float halfHeight() const;

    public:
        Enemy();

        float radius = 0;
        bool shouldKill = false;
        bool isActive = false;
        bool isActing = false;
        EnemyType enemyType = None;

        Enemy *getNext() const;
        void setNext(Enemy *);
        sf::Vector2f getPosition() const;
        sf::Vector2f getVelocity() const;
        void activateSeeker();
        void activateWanderer();
        void activateDodger();
        void pushApartBy(const Enemy &);
        void applyForce(sf::Vector2f);
        void reset();
        bool canAct();
        void markForKill();
        void killAddPoints();
        void update();
        void draw() const;
    };

    static constexpr int MAX_ENEMY_COUNT = 200;
    Enemy *firstAvailable {nullptr};
    float seekerSpawnChance = 60.f;
    float wandererSpawnChance = 60.f;
    float dodgerSpawnChance = 100.f;

    void checkSpawnSeeker();
    void checkSpawnWanderer();
    void checkSpawnDodger();
    void resetEnemyPool();
    void resetSpawnChances();

public:
    Enemies();

    static Enemies &instance() {
        static auto *instance = new Enemies;
        return *instance;
    }

    std::array<Enemy, MAX_ENEMY_COUNT> enemies {};

    std::default_random_engine randEngine {std::random_device{}()};
    std::uniform_real_distribution<float> directionDistribution {0.f, PI * 2 };

    void killAll();
    void update();
    void draw() const;
};



#endif //ENEMIES_H
