#include <cmath>
#include "../Include/Collisions.h"
#include "../Include/Player/Buffs.h"
#include "../Include/Player/PlayerStatus.h"
#include "../../Core/Include/Extensions.h"
#include "../Include/BlackHoles.h"
#include "../Include/Bullets.h"
#include "../Include/Enemies.h"
#include "../Include/Nukes.h"
#include "../Include/ShapeKeeper/ShapeKeeper.h"
#include "../Include/Player/PlayerShip.h"


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
                enemy.markForKill();

            // Check other enemies to push each other apart
            for (int j = i + 1; j < enemies.size(); j++)
                if (auto &enemy2 = enemies.at(j); enemy2.isActive && enemy2.isActing)
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
                        const sf::Vector2f v = bullet.getVelocity() - enemy.getCurrentVelocity();

                        // Only dodge the bullet if the bullet is traveling towards the dodger
                        if (p.dot(v) < 0.f)
                        {
                            const sf::Vector2f direction = enemy.getPosition() - bullet.getPosition();
                            enemy.applyForce(direction * 0.015f);
                        }
                    }

                    // Check if it is colliding
                    if (isColliding(bullet.radius + enemy.radius, bullet.getPosition(), enemy.getPosition()))
                    {
                        bullet.markForBlowUp();

                        // Enemy bodies and tails are indestructible from bullets
                        if (!enemy.isSnakeBody())
                            enemy.markForKill();
                    }
                }

            // Check player
            if (isColliding(enemy.radius + PlayerShip::instance().radius, enemy.getPosition(), PlayerShip::instance().getPosition()))
            {
                enemy.markForKill();
                if (!PlayerShip::instance().isInvincible)
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
                    if (!PlayerShip::instance().isInvincible)
                        PlayerStatus::instance().markForKill();
                }
            }
        }
}


void Collisions::handlePlayerAndBuffs()
{
    if (Buffs::instance().canPickUpBuff())
    {
        // Buff drop 1
        if (Buffs::instance().buffDrop1.availableForPickUp())
            if (isColliding(PlayerShip::instance().radius + Buffs::instance().buffDrop1.radius,
                    PlayerShip::instance().getPosition(),
                    Buffs::instance().buffDrop1.getPosition()))
                Buffs::instance().buffDrop1.isPickedUp = true;

        // Buff drop 2
        if (Buffs::instance().buffDrop2.availableForPickUp())
            if (isColliding(PlayerShip::instance().radius + Buffs::instance().buffDrop2.radius,
                    PlayerShip::instance().getPosition(),
                    Buffs::instance().buffDrop2.getPosition()))
                Buffs::instance().buffDrop2.isPickedUp = true;

        // Buff drop 3
        if (Buffs::instance().buffDrop3.availableForPickUp())
            if (isColliding(PlayerShip::instance().radius + Buffs::instance().buffDrop3.radius,
                    PlayerShip::instance().getPosition(),
                    Buffs::instance().buffDrop3.getPosition()))
                Buffs::instance().buffDrop3.isPickedUp = true;
    }
}


void Collisions::handlePlayerAndShapeKeeper()
{
    if (!ShapeKeeper::instance().isActive)
        return;

    auto &bullets = Bullets::instance().bullets;
    auto &lasers = ShapeKeeper::instance().lasersAttack.lasers;
    auto &core = ShapeKeeper::instance().core;
    auto &top = ShapeKeeper::instance().top;
    auto &middleLeft = ShapeKeeper::instance().middleLeft;
    auto &middleRight = ShapeKeeper::instance().middleRight;
    auto &bottomLeft = ShapeKeeper::instance().bottomLeft;
    auto &bottomRight = ShapeKeeper::instance().bottomRight;

    // Check all the bullets against each boss part
    // Short circuit each iteration if a bullet hits a part
    for (int b = 0; b < bullets.size(); b++)
        if (auto &bullet = bullets.at(b); bullet.isActive)
        {
            // Top boss part
            if (top.isAlive() && isColliding(bullet.radius + top.radius, bullet.getPosition(), top.getPosition()))
            {
                top.markForHit(bullet.getPosition(), 1);
                bullet.markForBlowUp();
                continue;
            }

            // Middle left boss part
            if (middleLeft.isAlive() && isColliding(bullet.radius + middleLeft.radius, bullet.getPosition(), middleLeft.getPosition()))
            {
                middleLeft.markForHit(bullet.getPosition(), 1);
                bullet.markForBlowUp();
                continue;
            }

            // Middle right boss part
            if (middleRight.isAlive() && isColliding(bullet.radius + middleRight.radius, bullet.getPosition(), middleRight.getPosition()))
            {
                middleRight.markForHit(bullet.getPosition(), 1);
                bullet.markForBlowUp();
                continue;
            }

            // Bottom left boss part
            if (bottomLeft.isAlive() && isColliding(bullet.radius + bottomLeft.radius, bullet.getPosition(), bottomLeft.getPosition()))
            {
                bottomLeft.markForHit(bullet.getPosition(), 1);
                bullet.markForBlowUp();
                continue;
            }

            // Bottom right boss part
            if (bottomRight.isAlive() && isColliding(bullet.radius + bottomRight.radius, bullet.getPosition(), bottomRight.getPosition()))
            {
                bottomRight.markForHit(bullet.getPosition(), 1);
                bullet.markForBlowUp();
                continue;
            }

            // Core collision
            if (ShapeKeeper::instance().canTakeCoreDamage() && core.isAlive() && isColliding(bullet.radius + core.radius, bullet.getPosition(), core.getPosition()))
            {
                core.markForHit(bullet.getPosition(), 1);
                bullet.markForBlowUp();
            }

            // Shield collision, just kill the bullet
            if (!ShapeKeeper::instance().canTakeCoreDamage() && core.isAlive() && isColliding(bullet.radius + core.shieldRadius, bullet.getPosition(), core.shield.getPosition()))
                bullet.markForBlowUp();

            // Collision with lasers
            if (ShapeKeeper::instance().lasersAttack.areLasersActive())
                for (int l = 0; l < lasers.size(); l++)
                    for (int c = 0; c < lasers.at(l).colliders.size(); c++)
                        if (isColliding(bullet.radius + lasers.at(l).colliders.at(c).getRadius(), bullet.getPosition(), lasers.at(l).colliders.at(c).getPosition()))
                            bullet.markForBlowUp();
        }

    // Check nukes. While a nuke is detonating, make sure it only hits the part once. Otherwise, just make sure
    // the hasBeenHitByNukes set to false. This is a weird place to do this but good enough for now
    if (Nukes::instance().isDetonating)
    {
        if (!top.hasBeenHitByNuke && top.isAlive() && isColliding(top.radius + Nukes::instance().radius, top.getPosition(), Nukes::instance().getPosition()))
        {
            top.hasBeenHitByNuke = true;
            top.markForHit(
                calculateCircleCollisionPoint(Nukes::instance().getPosition(), top.getPosition(), top.radius),
                10
            );
        }

        if (!middleLeft.hasBeenHitByNuke && middleLeft.isAlive() && isColliding(middleLeft.radius + Nukes::instance().radius, middleLeft.getPosition(), Nukes::instance().getPosition()))
        {
            middleLeft.hasBeenHitByNuke = true;
            middleLeft.markForHit(
                calculateCircleCollisionPoint(Nukes::instance().getPosition(), middleLeft.getPosition(), middleLeft.radius),
                10
            );
        }

        if (!middleRight.hasBeenHitByNuke && middleRight.isAlive() && isColliding(middleRight.radius + Nukes::instance().radius, middleRight.getPosition(), Nukes::instance().getPosition()))
        {
            middleRight.hasBeenHitByNuke = true;
            middleRight.markForHit(
                calculateCircleCollisionPoint(Nukes::instance().getPosition(), middleRight.getPosition(), middleRight.radius),
                10
            );
        }

        if (!bottomLeft.hasBeenHitByNuke && bottomLeft.isAlive() && isColliding(bottomLeft.radius + Nukes::instance().radius, bottomLeft.getPosition(), Nukes::instance().getPosition()))
        {
            bottomLeft.hasBeenHitByNuke = true;
            bottomLeft.markForHit(
                calculateCircleCollisionPoint(Nukes::instance().getPosition(), bottomLeft.getPosition(), bottomLeft.radius),
                10
            );
        }

        if (!bottomRight.hasBeenHitByNuke && bottomRight.isAlive() && isColliding(bottomRight.radius + Nukes::instance().radius, bottomRight.getPosition(), Nukes::instance().getPosition()))
        {
            bottomRight.hasBeenHitByNuke = true;
            bottomRight.markForHit(
                calculateCircleCollisionPoint(Nukes::instance().getPosition(), bottomRight.getPosition(), bottomRight.radius),
                10
            );
        }

        if (ShapeKeeper::instance().canTakeCoreDamage() && !core.hasBeenHitByNuke && core.isAlive() && isColliding(core.radius + Nukes::instance().radius, core.getPosition(), Nukes::instance().getPosition()))
        {
            core.hasBeenHitByNuke = true;
            core.markForHit(
                calculateCircleCollisionPoint(Nukes::instance().getPosition(), core.getPosition(), core.radius),
                10
            );
        }
    }
    else
    {
        top.hasBeenHitByNuke = false;
        middleLeft.hasBeenHitByNuke = false;
        middleRight.hasBeenHitByNuke = false;
        bottomLeft.hasBeenHitByNuke = false;
        bottomRight.hasBeenHitByNuke = false;
        core.hasBeenHitByNuke = false;
    }
}


sf::Vector2f Collisions::calculateCircleCollisionPoint(const sf::Vector2f& from, const sf::Vector2f& to, const float radius)
{
    sf::Vector2f direction = from - to;

    if (direction.lengthSquared() > 1.f)
        direction = direction.normalized();

    return direction * radius + to;
}
