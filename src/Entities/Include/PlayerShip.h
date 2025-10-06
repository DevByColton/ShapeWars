#ifndef PLAYERSHIP_H
#define PLAYERSHIP_H
#include "../../Content/Include/Art.h"
#include "SFML/Graphics/Sprite.hpp"


class PlayerShip {
private:
    const float SPAWN_BULLET_COOLDOWN = 0.1f;
    float timeUntilBulletSpawn = SPAWN_BULLET_COOLDOWN;
    const float BASE_SPEED = 8;
    float speed = BASE_SPEED;
    sf::Vector2f velocity {0.0, 0.0};
    sf::Vector2f spriteSizeF {0.0, 0.0};
    sf::Sprite ship {Art::instance().player};
    sf::Sprite invincibleShip {Art::instance().invincibleBuff};
    const sf::Color WHITE_SHADE {179, 179, 179, 255};
    const sf::Color DEEP_RED {178, 27, 7, 255};
    const sf::Color ORANGE_YELLOW {255, 187, 30, 255};

    float halfWidth() const;
    float halfHeight() const;
    void createShipExhaust() const;
    void createBulletGroups();

public:
    PlayerShip();

    static PlayerShip &instance() {
        static auto *instance = new PlayerShip;
        return *instance;
    }

    float radius = 20;
    bool isInvincible = false;
    bool isUsingBoosters = false;
    bool isUsingShotgunBullets = false;
    bool isUsingBulletsAllDirections = false;

    sf::Vector2f getPosition() const;
    void applyForce(sf::Vector2f);
    void setInvincibility(bool isInvincible);
    void setUsingBoosters(bool isUsingBoosters);
    void setUsingShotgunBullets(bool isUsingShotgunBullets);
    void setUsingBulletsAllDirections(bool isUsingBulletsAllDirections);
    void centerPlayer();
    void update();
    void draw() const;
};



#endif //PLAYERSHIP_H
