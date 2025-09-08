#ifndef PLAYERSHIP_H
#define PLAYERSHIP_H
#include "../../Content/Include/Art.h"
#include "SFML/Graphics/Sprite.hpp"


class PlayerShip {
private:
    sf::Sprite sprite {Art::instance().player};
    const int spawnBulletCooldownFrames = 6;
    int spawnBulletCooldownRemaining = 0;

    const sf::Color white {179, 179, 179, 255};
    const sf::Color deepRed {178, 27, 7, 255};
    const sf::Color orangeYellow {255, 187, 30, 255};
    sf::Vector2f velocity {0.0, 0.0};
    sf::Vector2f spriteSizeF {0.0, 0.0};
    float speed = 8;

    float halfWidth() const;
    float halfHeight() const;
    void createShipExhaust() const;

public:
    PlayerShip();
    ~PlayerShip() = default;

    static PlayerShip &instance() {
        static auto *instance = new PlayerShip;
        return *instance;
    }

    float radius = 20;

    void centerPlayer();
    sf::Vector2f getPosition() const;
    void applyForce(sf::Vector2f);
    void update();
    void draw() const;
};



#endif //PLAYERSHIP_H
