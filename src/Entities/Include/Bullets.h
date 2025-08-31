#ifndef BULLETS_H
#define BULLETS_H
#include <array>
#include <random>
#include "../../Content/Art.h"
#include "SFML/Graphics/Sprite.hpp"

class Bullets {
private:
    int nextBulletIndex = 0;
    void incrementNextBulletIndex();

public:
    Bullets() = default;
    ~Bullets() = default;

    static Bullets &instance() {
        static auto *instance = new Bullets;
        return *instance;
    }

    struct Bullet {
        Bullet() = default;
        ~Bullet() = default;

        sf::Sprite sprite {Art::instance().bullet};
        sf::Vector2f velocity {0.0, 0.0};
        float radius = 8;
        bool isActive = false;

        sf::Vector2f getPosition() const;
        void reset();
    };

    std::array<Bullet, 400> bullets {};
    std::default_random_engine randEngine {std::random_device{}()};
    std::uniform_real_distribution<float> spreadDistribution {-0.04f, 0.04f};

    void resetAll();
    void addBulletGroup(sf::Vector2f, sf::Vector2f);
    void update();
    void draw();
};



#endif //BULLETS_H
