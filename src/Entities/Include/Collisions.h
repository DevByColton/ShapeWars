#ifndef COLLISIONS_H
#define COLLISIONS_H
#include "SFML/System/Vector2.hpp"


struct Collisions {
    Collisions() = default;
    ~Collisions() = default;

    static Collisions &instance() {
        static auto *instance = new Collisions;
        return *instance;
    }

    static constexpr float SMALL_PROXIMITY_RADIUS = 150.0;
    static constexpr float SMALL_PROXIMITY_RADIUS_SQR = SMALL_PROXIMITY_RADIUS * SMALL_PROXIMITY_RADIUS;
    static constexpr float LARGE_PROXIMITY_RADIUS = 250.0;
    static constexpr float LARGE_PROXIMITY_RADIUS_SQR = LARGE_PROXIMITY_RADIUS * LARGE_PROXIMITY_RADIUS;


    bool isColliding(float, sf::Vector2f, sf::Vector2f);
    void handleEnemyPlayerBullets();
    void handleBlackHoles();
};



#endif //COLLISIONS_H
