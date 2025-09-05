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

    public:
        BlackHole();

        float radius = 0.0;
        bool isActive = false;

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

    static constexpr int MAX_BLACK_HOLE_COUNT = 4;
    BlackHole *firstAvailable {nullptr};
    std::default_random_engine randEngine {std::random_device{}()};
    std::uniform_int_distribution<int> spawnDistribution {0, 600};

    void spawnBlackHole();
    void resetBlackHolePool();

public:
    BlackHoles();

    static BlackHoles &instance()
    {
        static auto *instance = new BlackHoles;
        return *instance;
    }

    std::array<BlackHole, MAX_BLACK_HOLE_COUNT> blackHoles {};

    void resetAll();
    void killAll();
    void update();
    void draw() const;
};


#endif //BLACKHOLES_H
