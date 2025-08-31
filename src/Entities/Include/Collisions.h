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

    bool isColliding(float, sf::Vector2f, sf::Vector2f);
    void handleEnemyAndPlayer();
};



#endif //COLLISIONS_H
