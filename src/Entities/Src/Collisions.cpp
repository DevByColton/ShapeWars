#include <cmath>
#include "../Include/Collisions.h"
#include "../../PlayerStatus/PlayerStatus.h"
#include "../../System/Include/Extensions.h"
#include "../Include/BlackHoles.h"
#include "../Include/Bullets.h"
#include "../Include/Enemies.h"
#include "../Include/Nukes.h"
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
        if (auto &enemy = enemies.at(i); (enemy.isActive && enemy.isActing) || (enemy.isActive && Nukes::instance().isDetonating))
        {
            // Check nuke if active
            if (Nukes::instance().isDetonating && isColliding(enemy.radius + Nukes::instance().radius, enemy.getPosition(), Nukes::instance().getPosition()))
            {
                enemy.markForKill();
                continue;
            }

            // Check other enemies to push each other apart
            for (int j = i + 1; j < enemies.size(); j++)
                if (auto &enemy2 = enemies.at(j); enemy2.isActive)
                    if (isColliding(enemy.radius + enemy2.radius, enemy.getPosition(), enemy2.getPosition())) {
                        enemy.pushApartBy(enemy2);
                        enemy2.pushApartBy(enemy);
                    }

            // Check bullets
            for (int b = 0; b < bullets.size(); b++)
                if (auto &bullet = bullets.at(b); bullet.isActive)
                {
                    // Special case for dodger enemies, see if they are in proximity to dodge the bullet
                    if (enemy.enemyType == Dodger && Extensions::distanceSquared(enemy.getPosition(), bullet.getPosition()) < SMALL_PROXIMITY_RADIUS_SQR)
                    {
                        const sf::Vector2f p = bullet.getPosition() - enemy.getPosition();
                        const sf::Vector2f v = bullet.getVelocity() - enemy.getVelocity();

                        // Only dodge the bullet if the bullet is traveling towards the dodger
                        if (p.dot(v) < 0.f)
                        {
                            const sf::Vector2f direction = enemy.getPosition() - bullet.getPosition();
                            enemy.applyForce(direction * 0.015f);
                        }
                    }

                    // Check if it is colliding
                    if (isColliding(bullet.radius + enemy.radius, bullet.getPosition(), enemy.getPosition())) {
                        enemy.markForKill();
                        bullet.markForBlowUp();
                        break;
                    }
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
            // Check nuke if active
            if (Nukes::instance().isDetonating && isColliding(blackHole.radius + Nukes::instance().radius, blackHole.getPosition(), Nukes::instance().getPosition()))
            {
                blackHole.markHit(true);
                continue;
            }

            // Check bullets
            for (int b = 0; b < bullets.size(); b++)
            {
                if (auto &bullet = bullets.at(b); bullet.isActive)
                {
                    // See if the bullet is in proximity
                    if (Extensions::distanceSquared(blackHole.getPosition(), bullet.getPosition()) < MEDIUM_PROXIMITY_RADIS_SQR)
                    {
                        // Apply repulsive constant force, normalize to make sure all angles apply the same force
                        sf::Vector2f direction = bullet.getPosition() - blackHole.getPosition();
                        if (direction.lengthSquared() > 1.f)
                        {
                            direction = direction.normalized() * 1.8f;
                            bullet.applyForce(direction);
                        }

                        if (isColliding(bullet.radius + blackHole.radius, bullet.getPosition(), blackHole.getPosition()))
                        {
                            blackHole.markHit(false);
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
                const float lerpValue = std::lerp(3.f, 0.f, direction.length() / LARGE_PROXIMITY_RADIUS);
                direction *= lerpValue * 0.02f;
                PlayerShip::instance().applyForce(direction);

                // Check collision with player ship
                if (isColliding(PlayerShip::instance().radius + blackHole.radius, PlayerShip::instance().getPosition(), blackHole.getPosition()))
                {
                    blackHole.markHit(false);
                    PlayerStatus::instance().markForKill();
                }
            }
        }
}

