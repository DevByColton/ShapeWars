#ifndef BULLETS_H
#define BULLETS_H
#include <array>
#include <random>
#include "../../Content/Include/Art.h"
#include "SFML/Graphics/Sprite.hpp"

class Bullets {
private:
    class Bullet {
    private:
        union
        {
            struct
            {
                float xVelocity;
                float yVelocity;
            };

            Bullet *next {nullptr};
        };

        bool shouldBlowUp = false;

    public:
        Bullet();

        float radius = 0.0;
        bool isActive = false;
        sf::Sprite sprite {Art::instance().bullet};

        Bullet *getNext() const;
        void setNext(Bullet *);
        sf::Vector2f getPosition() const;
        sf::Vector2f getVelocity() const;
        void activate(sf::Vector2f, sf::Vector2f);
        void applyForce(sf::Vector2f);
        void blowUp();
        bool getShouldBlowUp() const;
        void markForBlowUp();
        void update();
        void draw() const;
    };

    static constexpr int MAX_BULLET_COUNT = 1001;
    Bullet *firstAvailable {nullptr};

    void resetBulletPool();

public:
    Bullets();

    static Bullets &instance() {
        static auto *instance = new Bullets;
        return *instance;
    }

    sf::Color bulletExplosionColor {255, 197, 51, 255};
    std::array<Bullet, MAX_BULLET_COUNT> bullets {};
    std::default_random_engine randEngine {std::random_device{}()};
    std::uniform_real_distribution<float> spreadDistribution {-0.04f, 0.04f};

    void resetAll();
    void addBulletGroup(const sf::Vector2f& position, float direction);
    void update();
    void draw() const;
};



#endif //BULLETS_H
