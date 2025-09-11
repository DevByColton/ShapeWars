#include <cmath>
#include "../Include/Collisions.h"
#include "../../GameRoot.h"
#include "../../PlayerStatus/PlayerStatus.h"
#include "../../System/Include/Extensions.h"
#include "../Include/BlackHoles.h"
#include "../Include/Bullets.h"
#include "../Include/Enemies.h"
#include "../Include/PlayerShip.h"


bool Collisions::isColliding(const float radii, const sf::Vector2f position1, const sf::Vector2f position2)
{
    return Extensions::distanceSquared(position1, position2) < radii * radii;
}


void Collisions::handleEnemyPlayerBullets()
{
    auto &enemies = Enemies::instance().enemies;
    auto &bullets = Bullets::instance().bullets;

    // Loop through all the enemies once to check other enemies, the player ship, and bullets
    for (int i = 0; i < enemies.size(); i++)
    {
        // Check enemies
        if (auto &enemy = enemies.at(i); enemy.isActive && enemy.isActing)
        {
            // Check other enemies
            for (int j = i + 1; j < enemies.size(); j++)
                if (auto &enemy2 = enemies.at(j); enemy2.isActive)
                    if (isColliding(enemy.radius + enemy2.radius, enemy.getPosition(), enemy2.getPosition())) {
                        enemy.pushApartBy(enemy2);
                        enemy2.pushApartBy(enemy);
                    }

            // Check bullets
            for (int b = 0; b < bullets.size(); b++)
                if (auto &bullet = bullets.at(b); bullet.isActive)
                    if (isColliding(bullet.radius + enemy.radius, bullet.getPosition(), enemy.getPosition())) {
                        enemy.markForKill();
                        bullet.markForBlowUp();
                    }

            // Check player
            if (isColliding(enemy.radius + PlayerShip::instance().radius, enemy.getPosition(), PlayerShip::instance().getPosition()))
            {
                enemy.markForKill();
                PlayerStatus::instance().markForKill();
            }
        }
    }
}


void Collisions::handleBlackHoles()
{
    auto &bullets = Bullets::instance().bullets;
    auto &blackHoles = BlackHoles::instance().blackHoles;

    for (int bh = 0; bh < blackHoles.size(); bh++)
        if (auto &blackHole = blackHoles.at(bh); blackHole.isActive)
        {
            // Check bullets
            for (int b = 0; b < bullets.size(); b++)
            {
                if (auto &bullet = bullets.at(b); bullet.isActive)
                {
                    // See if the bullet is in proximity
                    if (Extensions::distanceSquared(blackHole.getPosition(), bullet.getPosition()) < SMALL_PROXIMITY_RADIUS_SQR)
                    {
                        // Apply repulsive constant force
                        sf::Vector2f direction = bullet.getPosition() - blackHole.getPosition();
                        direction *= GameRoot::instance().deltaTime;
                        bullet.applyForce(direction);

                        if (isColliding(bullet.radius + blackHole.radius, bullet.getPosition(), blackHole.getPosition()))
                        {
                            blackHole.markHit();
                            bullet.markForBlowUp();
                        }
                    }
                }
            }

            // Check player
            if (Extensions::distanceSquared(blackHole.getPosition(), PlayerShip::instance().getPosition()) < LARGE_PROXIMITY_RADIUS_SQR)
            {
                // Apply attractive linear force
                sf::Vector2f direction = blackHole.getPosition() - PlayerShip::instance().getPosition();
                const float lerpValue = std::lerp(3.0, 0.0, direction.length() / LARGE_PROXIMITY_RADIUS);
                direction *= lerpValue * GameRoot::instance().deltaTime;
                PlayerShip::instance().applyForce(direction);

                // Check collision with player ship
                if (isColliding(PlayerShip::instance().radius + blackHole.radius, PlayerShip::instance().getPosition(), blackHole.getPosition()))
                {
                    blackHole.markHit();
                    PlayerStatus::instance().markForKill();
                }
            }
        }
}

