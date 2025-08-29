#ifndef PLAYERSHIP_H
#define PLAYERSHIP_H
#include "../../Content/Art.h"
#include "SFML/Graphics/Sprite.hpp"


class PlayerShip {
private:
    sf::Sprite sprite {Art::instance().Player};
    const int spawnBulletCooldownFrames = 6;
    int spawnBulletCooldownRemaining = 0;

public:
    PlayerShip();
    ~PlayerShip() = default;

    static PlayerShip &instance() {
        static auto *instance = new PlayerShip;
        return *instance;
    }

    sf::Vector2f velocity {0.0, 0.0};
    sf::Vector2f spriteSizeF {0.0, 0.0};
    float speed = 8;
    float radius = 20;

    sf::Vector2f getPosition() const;
    sf::Vector2<int> size() const;
    float halfWidth() const;
    float halfHeight() const;

    void update();
    void draw() const;
};



#endif //PLAYERSHIP_H
