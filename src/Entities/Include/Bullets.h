#ifndef BULLETS_H
#define BULLETS_H
#include <array>
#include <random>
#include "../../Content/Art.h"
#include "SFML/Graphics/Sprite.hpp"

class Bullets {
private:
    struct Bullet {
        Bullet() = default;
        ~Bullet() = default;

        sf::Sprite sprite {Art::instance().Bullet};
        sf::Vector2f velocity {0.0, 0.0};
        float radius = 8;
        bool isActive = false;

        sf::Vector2f getPosition() const;
        void reset();
    };

    int nextBulletIndex = 0;
    std::array<Bullet, 400> bullets {};

    void incrementNextBulletIndex();

public:
    Bullets() = default;
    ~Bullets() = default;

    static Bullets &instance() {
        static auto *instance = new Bullets;
        return *instance;
    }

    std::default_random_engine randEngine {std::random_device{}()};
    std::uniform_real_distribution<float> spreadDistribution {-0.04f, 0.04f};

    void addBulletGroup(sf::Vector2f, sf::Vector2f);
    void update();
    void draw() const;
};



#endif //BULLETS_H
