#ifndef PLAYERSHIP_H
#define PLAYERSHIP_H
#include "../../Content/Art.h"
#include "SFML/Graphics/Sprite.hpp"


class PlayerShip {
private:
    sf::Sprite sprite {Art::instance().player};
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
    float timeUntilRespawn = 0.0f;
    int lives = 3;
    float speed = 8;
    float radius = 20;

    sf::Vector2f getPosition() const;
    sf::Vector2<int> size() const;
    float halfWidth() const;
    float halfHeight() const;
    bool isDead() const;
    void kill();
    void update();
    void draw() const;
};



#endif //PLAYERSHIP_H
