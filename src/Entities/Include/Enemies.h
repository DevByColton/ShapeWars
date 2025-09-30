#ifndef ENEMIES_H
#define ENEMIES_H
#include <array>
#include <functional>
#include <random>
#include "../../Content/Include/Art.h"
#include "../../System/Include/Extensions.h"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/System/Vector2.hpp"


enum EnemyType
{
    None,
    Dodger,
    Seeker,
    Wanderer,
    SnakeHead,
    SnakeBody,
    SnakeTail
};


class Enemies {
private:
    class Enemy {
    private:
        union
        {
            struct
            {
                int pointValue;
                float xVelocity;
                float yVelocity;
                float turnXVelocity;
                float turnYVelocity;
                float speed;
                float timeUntilAct;
                float timeUntilNewDirection;
                float maxTimeUntilNewDirection;
            };

            Enemy *next {nullptr};
        };

        const float maxTimeUntilAct = 1.f;
        std::function<void()> behavior;
        sf::Vector2f spriteSizeF {0.0, 0.0};
        sf::Sprite sprite {Art::instance().enemyPlaceholder};
        Enemy* trailing {nullptr};
        Enemy* leading {nullptr};

        float halfWidth() const;
        float halfHeight() const;

    public:
        float radius = 0;
        bool shouldKill = false;
        bool isActive = false;
        bool isActing = false;
        EnemyType enemyType = None;

        Enemy *getNext() const;
        void setNext(Enemy *);
        void setLeading(Enemy *);
        void setTrailing(Enemy *);
        sf::Vector2f getPosition() const;
        sf::Vector2f getCurrentVelocity() const;
        sf::Vector2f getTurnVelocity() const;
        bool isSnakeType() const;
        bool isSnakeBody() const;
        void activateSeeker();
        void activateWanderer();
        void activateDodger();
        void activateSnakeHead();
        void activateSnakeBodyPart(EnemyType enemyType);
        void pushApartBy(const Enemy &);
        void applyForce(sf::Vector2f);
        void reset();
        bool canAct();
        void markForKill();
        void killAddPoints();
        void update();
        void draw() const;
    };

    static constexpr int MAX_ENEMY_COUNT = 400;
    float seekerSpawnChance = 60.f;
    float wandererSpawnChance = 60.f;
    float dodgerSpawnChance = 100.f;
    float snakeSpawnChance = 200.f;
    Enemy *firstAvailable {nullptr};

    void checkSeekerSpawn();
    void checkWandererSpawn();
    void checkDodgerSpawn();
    void checkSnakeSpawn();
    void resetEnemyPool();
    void resetSpawnChances();

public:
    Enemies();

    static Enemies &instance() {
        static auto *instance = new Enemies;
        return *instance;
    }

    bool canSpawn = true;
    std::array<Enemy, MAX_ENEMY_COUNT> enemies {};
    std::default_random_engine randEngine {std::random_device{}()};
    std::uniform_real_distribution<float> directionDistribution {0.f, PI * 2.f};
    std::uniform_int_distribution<int> snakeBodyPartCountDistribution {6, 20};

    void killAll();
    void update();
    void draw() const;
};



#endif //ENEMIES_H
