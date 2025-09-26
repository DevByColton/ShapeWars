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

        bool wasHit = false;
        sf::Sprite sprite {Art::instance().blackHole};
        sf::Color particleSprayColor {115, 255, 173, 255};
        sf::Color hitColor {255, 124, 115, 255};
        const int hitParticleCount = 150;

    public:
        BlackHole();

        const float maxParticleSprayTime = 0.025f;
        float particleSprayTime = maxParticleSprayTime;
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
        void markHit();
        bool hit();
        void kill(bool);
        void update();
        void draw() const;
    };

    BlackHole *firstAvailable {nullptr};
    std::default_random_engine randEngine {std::random_device{}()};
    std::uniform_int_distribution<int> spawnDistribution {0, 70};

    void spawnBlackHole();
    void resetBlackHolePool();

public:
    BlackHoles();

    static BlackHoles &instance()
    {
        static auto *instance = new BlackHoles;
        return *instance;
    }

    static constexpr int MAX_BLACK_HOLE_COUNT = 4;
    std::array<BlackHole, MAX_BLACK_HOLE_COUNT> blackHoles {};

    void resetAll();
    void killAll();
    void update();
    void draw() const;
};


#endif //BLACKHOLES_H
