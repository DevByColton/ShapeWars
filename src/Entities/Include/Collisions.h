#ifndef COLLISIONS_H
#define COLLISIONS_H
#include "Bullets.h"
#include "SFML/System/Vector2.hpp"
#include "ShapeKeeper/ShapeKeeperBodyPart.h"


class Collisions {
private:
    static sf::Vector2f calculateCircleCollisionPoint(const sf::Vector2f& from, const sf::Vector2f& to, float radius);

    void checkPlayer(ShapeKeeperBodyPart& part);
    bool check(ShapeKeeperBodyPart& part, Bullets::Bullet& bullet);
    void checkNuke(ShapeKeeperBodyPart& part);

public:
    static Collisions &instance() {
        static auto *instance = new Collisions;
        return *instance;
    }

    static constexpr float SMALL_PROXIMITY_RADIUS = 150.f;
    static constexpr float SMALL_PROXIMITY_RADIUS_SQR = SMALL_PROXIMITY_RADIUS * SMALL_PROXIMITY_RADIUS;
    static constexpr float MEDIUM_PROXIMITY_RADIS = 162.f;
    static constexpr float MEDIUM_PROXIMITY_RADIS_SQR = MEDIUM_PROXIMITY_RADIS * MEDIUM_PROXIMITY_RADIS;
    static constexpr float LARGE_PROXIMITY_RADIUS = 250.f;
    static constexpr float LARGE_PROXIMITY_RADIUS_SQR = LARGE_PROXIMITY_RADIUS * LARGE_PROXIMITY_RADIUS;

    bool isColliding(float, sf::Vector2f, sf::Vector2f);
    void handleEnemyPlayerBullets();
    void handleBlackHoles();
    void handlePlayerAndBuffs();
    void handleShapeKeeper();
};



#endif //COLLISIONS_H
