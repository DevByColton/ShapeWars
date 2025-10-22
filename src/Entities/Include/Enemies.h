#ifndef ENEMIES_H
#define ENEMIES_H
#include <array>
#include <functional>
#include <random>
#include "../../Content/Include/Art.h"
#include "../../Core/Include/Extensions.h"
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
        void reset(bool canDropBuffChance);
        bool canAct();
        void markForKill();
        void killAddPoints();
        void update();
        void draw() const;
    };

    // Wanderer spawn variables
    std::default_random_engine wandererRandEngine {std::random_device{}()};
    std::uniform_int_distribution<int> wandererSpawnDistribution {1, 100};
    static constexpr float WANDERER_SPAWN_INTERVAL = .25f;
    float timeUntilWandererSpawn = WANDERER_SPAWN_INTERVAL;
    static constexpr float WANDERER_SPAWN_RATE_INCREASE_INTERVAL = 10.f;
    float timeUntilWandererSpawnIncrease = WANDERER_SPAWN_RATE_INCREASE_INTERVAL;
    static constexpr int MAX_WANDERER_SPAWN_RATE = 50;
    static constexpr int STARTING_WANDERER_SPAWN_RATE = 25;
    int wandererSpawnRate = STARTING_WANDERER_SPAWN_RATE;

    // Seeker spawn variables
    std::default_random_engine seekerRandEngine {std::random_device{}()};
    std::uniform_int_distribution<int> seekerSpawnDistribution {1, 100};
    static constexpr float SEEKER_SPAWN_INTERVAL = .5f;
    float timeUntilSeekerSpawn = SEEKER_SPAWN_INTERVAL;
    static constexpr float SEEKER_SPAWN_RATE_INCREASE_INTERVAL = 20.f;
    float timeUntilSeekerSpawnIncrease = SEEKER_SPAWN_RATE_INCREASE_INTERVAL;
    static constexpr int MAX_SEEKER_SPAWN_RATE = 30;
    static constexpr int STARTING_SEEKER_SPAWN_RATE = 10;
    int seekerSpawnRate = STARTING_SEEKER_SPAWN_RATE;

    // Dodger spawn variables, start spawning after 30 seconds
    std::default_random_engine dodgerRandEngine {std::random_device{}()};
    std::uniform_int_distribution<int> dodgerSpawnDistribution {1, 100};
    static constexpr float TOTAL_TIME_UNTIL_DODGERS_CAN_SPAWN = 30.f;
    float timeUntilDodgersCanSpawn = TOTAL_TIME_UNTIL_DODGERS_CAN_SPAWN;
    static constexpr float DODGER_SPAWN_INTERVAL = .5f;
    float timeUntilDodgerSpawn = DODGER_SPAWN_INTERVAL;
    static constexpr float DODGER_SPAWN_RATE_INCREASE_INTERVAL = 20.f;
    float timeUntilDodgerSpawnIncrease = DODGER_SPAWN_RATE_INCREASE_INTERVAL;
    static constexpr int MAX_DODGER_SPAWN_RATE = 25;
    static constexpr int STARTING_DODGER_SPAWN_RATE = 10;
    int dodgerSpawnRate = STARTING_DODGER_SPAWN_RATE;

    // Snake spawn variables, start spawning after 60 seconds
    std::uniform_int_distribution<int> snakeBodyPartCountDistribution {6, 20};
    std::default_random_engine snakeRandEngine {std::random_device{}()};
    std::uniform_int_distribution<int> snakeSpawnDistribution {1, 100};
    static constexpr float TOTAL_TIME_UNTIL_SNAKES_CAN_SPAWN = 60.f;
    float timeUntilSnakesCanSpawn = TOTAL_TIME_UNTIL_SNAKES_CAN_SPAWN;
    static constexpr float SNAKE_SPAWN_INTERVAL = .5f;
    float timeUntilSnakeSpawn = SNAKE_SPAWN_INTERVAL;
    static constexpr float SNAKE_SPAWN_RATE_INCREASE_INTERVAL = 30.f;
    float timeUntilSnakeSpawnIncrease = SNAKE_SPAWN_RATE_INCREASE_INTERVAL;
    static constexpr int MAX_SNAKE_SPAWN_RATE = 70;
    static constexpr int STARTING_SNAKE_SPAWN_RATE = 20;
    int snakeSpawnRate = STARTING_SNAKE_SPAWN_RATE;

    static constexpr int MAX_ENEMY_COUNT = 600;
    Enemy *firstAvailable {nullptr};

    void checkSeekerSpawn();
    void checkWandererSpawn();
    void checkDodgerSpawn();
    void checkSnakeSpawn();
    void resetEnemyPool();

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

    void resetSpawnRates();
    void killAll();
    void update();
    void draw() const;
};



#endif //ENEMIES_H
