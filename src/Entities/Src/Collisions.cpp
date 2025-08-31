#include "../Include/Collisions.h"
#include "../../System/Include/Extensions.h"
#include "../Include/Bullets.h"
#include "../Include/Enemies.h"
#include "../Include/PlayerShip.h"


bool Collisions::isColliding(const float radii, const sf::Vector2f position1, const sf::Vector2f position2) {
    return distanceSquared(position1, position2) < radii * radii;
}


void Collisions::handleEnemyAndPlayer() {

    auto &enemies = Enemies::instance().enemies;
    auto &bullets = Bullets::instance().bullets;

    // Loop through all the enemies once to check other enemies, the player ship, and bullets
    for (std::size_t i = 0; i < enemies.size(); i++) {

        if (auto &enemy = enemies.at(i); enemy.isActive) {

            // Check other enemies
            for (std::size_t j = i + 1; j < enemies.size(); j++)
                if (auto &enemy2 = enemies.at(j); enemy2.isActive)
                    if (isColliding(enemy.radius + enemy2.radius, enemy.getPosition(), enemy2.getPosition())) {
                        enemy.pushApartBy(enemy2);
                        enemy2.pushApartBy(enemy);
                    }

            // Check bullets
            for (std::size_t b = 0; b < bullets.size(); b++)
                if (auto &bullet = bullets.at(b); bullet.isActive)
                    if (isColliding(bullet.radius + enemy.radius, bullet.getPosition(), enemy.getPosition())) {
                        enemy.killAddPoint();
                        bullet.reset();
                    }

            // Check player
            if (isColliding(enemy.radius + PlayerShip::instance().radius, enemy.getPosition(), PlayerShip::instance().getPosition())) {
                enemy.killAddPoint();
                PlayerShip::instance().kill();
            }
        }
    }
}
