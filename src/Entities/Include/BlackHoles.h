#ifndef BLACKHOLES_H
#define BLACKHOLES_H
#include <random>
#include "../../Content/Include/Art.h"
#include "SFML/Graphics/Sprite.hpp"


class BlackHoles {
private:
    class BlackHole
    {
    private:
        union
        {
            struct
            {
                int pointValue;
                int hitPoints;
            };

            BlackHole *next {nullptr};
        };

        bool wasKillShot = false;
        bool wasHit = false;
        sf::Sprite sprite {Art::instance().blackHole};
        sf::Color particleSprayColor {64, 255, 143, 255};
        sf::Color hitParticleColorPrimary {140, 255, 188, 255};
        sf::Color hitParticleColorSecondary {179, 255, 210, 255};
        const int hitParticleCount = 150;

    public:
        BlackHole();

        static constexpr float MAX_PARTICLE_SPRAY_TIME = 0.025f;
        float particleSprayTime = MAX_PARTICLE_SPRAY_TIME;
        float particleSprayAngle = 0.f;
        float radius = 0.0;
        bool isActive = false;
        std::uniform_real_distribution<float> magnitude {8.f, 15.f};
        std::uniform_real_distribution<float> sprayAngle {-4.f, 4.f};

        BlackHole* getNext() const;
        void setNext(BlackHole* next);
        sf::Vector2f getPosition() const;
        void activate();
        void reset();
        bool getWasHit() const;
        void markHit(bool wasKillShot);
        bool hit();
        void kill(bool);
        void update();
        void draw() const;
    };

    // Black hole spawn variables, start spawning after 45 seconds
    std::default_random_engine blackHoleRandEngine {std::random_device{}()};
    std::uniform_int_distribution<int> spawnDistribution {1, 100};
    static constexpr float TOTAL_TIME_UNTIL_CAN_SPAWN = 45.f;
    float timeUntilCanSpawn = TOTAL_TIME_UNTIL_CAN_SPAWN;
    static constexpr float SPAWN_INTERVAL = 5.f;
    float timeUntilSpawn = SPAWN_INTERVAL;
    static constexpr float SPAWN_RATE_INCREASE_INTERVAL = 20.f;
    float timeUntilSpawnIncrease = SPAWN_RATE_INCREASE_INTERVAL;
    static constexpr int MAX_SPAWN_RATE = 40;
    static constexpr int STARTING_SPAWN_RATE = 10;
    int spawnRate = STARTING_SPAWN_RATE;

    BlackHole *firstAvailable {nullptr};
    std::default_random_engine randEngine {std::random_device{}()};

    void spawnBlackHole();
    void resetBlackHolePool();

public:
    BlackHoles();

    static BlackHoles &instance()
    {
        static auto *instance = new BlackHoles;
        return *instance;
    }

    bool canSpawn = true;
    static constexpr int MAX_BLACK_HOLE_COUNT = 4;
    std::array<BlackHole, MAX_BLACK_HOLE_COUNT> blackHoles {};

    void resetSpawnRate();
    void killAll();
    void update();
    void draw() const;
};


#endif //BLACKHOLES_H
