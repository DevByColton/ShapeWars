#include <algorithm>
#include <cmath>
#include "../../Include/Player/PlayerShip.h"
#include "../../../GameRoot.h"
#include "../../../Content/Include/Art.h"
#include "../../../Content/Include/GaussianBlur.h"
#include "../../../Input/Include/Input.h"
#include "../../../Systems/Include/Particles.h"
#include "../../Include/Player/PlayerStatus.h"
#include "../../../Core/Include/Extensions.h"
#include "../../../Core/Include/RandomVector.h"
#include "../../Include/Bullets.h"

PlayerShip::PlayerShip()
{
    ship.setPosition(GameRoot::instance().windowSizeF / 2.f);
    ship.setOrigin({
        ship.getTexture().getSize().x / 2.f,
        ship.getTexture().getSize().y / 2.f
    });

    invincibleShip.setOrigin({
        invincibleShip.getTexture().getSize().x / 2.f,
        invincibleShip.getTexture().getSize().y / 2.f
    });
    invincibleShip.setPosition(ship.getPosition());
}


void PlayerShip::reset()
{
    // Resetting the player texture in case a buff was enabled on death
    speed = BASE_SPEED;
    ship.setTexture(Art::instance().player);
    ship.setOrigin({
        Art::instance().player.getSize().x / 2.f,
        Art::instance().player.getSize().y / 2.f
    });
    ship.setPosition(GameRoot::instance().windowSizeF / 2.f);
    ship.setRotation(sf::Angle::Zero);
    isInvincible = false;
    isUsingBoosters = false;
    isUsingBulletsAllDirections = false;
    isUsingShotgunBullets = false;
}


sf::Vector2f PlayerShip::getPosition() const
{
    return ship.getPosition();
}


float PlayerShip::halfWidth() const
{
    return ship.getTexture().getSize().x / 2.f;
}


float PlayerShip::halfHeight() const
{
    return ship.getTexture().getSize().y / 2.f;
}


void PlayerShip::applyForce(const sf::Vector2f amount)
{
    velocity += amount;
}


void PlayerShip::createShipExhaust() const
{
    if (velocity.lengthSquared() > 0.1f)
    {
        const float orientation = Extensions::toAngle(velocity);
        const Quaternion quaternion = Quaternion::createFromYawPitchRoll(0.f, 0.f, orientation);
        const sf::Vector2f baseVelocity = velocity * -0.5f;
        sf::Vector2f perpendicularVelocity = {baseVelocity.y, -baseVelocity.x};
        perpendicularVelocity = perpendicularVelocity * (0.6f * std::sin(GameRoot::instance().elapsedGameTime * 8.f));
        const sf::Vector2f exhaustPosition = getPosition() + Extensions::transform({-30.f, 0.f}, quaternion);

        // Center particle stream
        Particles::instance().create(
            0.5f,
            DontIgnoreGravity,
            Spark,
            exhaustPosition,
            baseVelocity + RandomVector::instance().next(0.f, 1.f),
            WHITE_SHADE,
            ORANGE_YELLOW
        );

        // Side particle streams with glow
        Particles::instance().create(
            0.5f,
            DontIgnoreGravity,
            Spark,
            exhaustPosition,
            baseVelocity + perpendicularVelocity,
            WHITE_SHADE,
            DEEP_RED
        );
        Particles::instance().create(
            0.5f,
            DontIgnoreGravity,
            Spark,
            exhaustPosition,
            baseVelocity - perpendicularVelocity,
            WHITE_SHADE,
            DEEP_RED
        );

        // Moooore sparks at the booster positions
        if (isUsingBoosters)
        {
            Particles::instance().create(
                0.5f,
                DontIgnoreGravity,
                Spark,
                getPosition() + Extensions::transform({-30.f, 12.f}, quaternion),
                baseVelocity + RandomVector::instance().next(0.f, 1.f),
                WHITE_SHADE,
                ORANGE_YELLOW
            );

            Particles::instance().create(
                0.5f,
                DontIgnoreGravity,
                Spark,
                getPosition() + Extensions::transform({-30.f, -12.f}, quaternion),
                baseVelocity + RandomVector::instance().next(0.f, 1.f),
                WHITE_SHADE,
                ORANGE_YELLOW
            );
        }
    }
}


void PlayerShip::createBulletGroups()
{
    // Check aiming direction and fire bullets after cooldown
    timeUntilBulletSpawn -= GameRoot::instance().deltaTime;
    const bool hasBulletSpawnTimeLapsed = timeUntilBulletSpawn < 0.f;
    const sf::Vector2f aimDirection = Input::instance().getAimDirection(getPosition());
    const bool isAimingInDirection = aimDirection.lengthSquared() > 0;
    float aimAngle = 0.f;

    if (isAimingInDirection)
        aimAngle = Extensions::toAngle(aimDirection);

    // Reset cooldown time
    if (hasBulletSpawnTimeLapsed)
        timeUntilBulletSpawn = SPAWN_BULLET_COOLDOWN;

    if (isAimingInDirection && hasBulletSpawnTimeLapsed)
    {
        // Fire bullet groups at 45 degree increments in a circle around the ship
        if (isUsingBulletsAllDirections)
        {
            Bullets::instance().addBulletGroup(getPosition(), aimAngle);
            Bullets::instance().addBulletGroup(getPosition(), aimAngle += FORTY_FIVE_DEGREES);
            Bullets::instance().addBulletGroup(getPosition(), aimAngle += FORTY_FIVE_DEGREES);
            Bullets::instance().addBulletGroup(getPosition(), aimAngle += FORTY_FIVE_DEGREES);
            Bullets::instance().addBulletGroup(getPosition(), aimAngle += FORTY_FIVE_DEGREES);
            Bullets::instance().addBulletGroup(getPosition(), aimAngle += FORTY_FIVE_DEGREES);
            Bullets::instance().addBulletGroup(getPosition(), aimAngle += FORTY_FIVE_DEGREES);
            Bullets::instance().addBulletGroup(getPosition(), aimAngle += FORTY_FIVE_DEGREES);
            return;
        }

        // Shotgun bullet groups, 3 groups with slightly different aim directions
        if (isUsingShotgunBullets)
        {
            // Top angled
            Bullets::instance().addBulletGroup(getPosition(), aimAngle - PI / 12.f);

            // Straight towards aim direction
            Bullets::instance().addBulletGroup(getPosition(), aimAngle);

            // Bottom angled
            Bullets::instance().addBulletGroup(getPosition(), aimAngle + PI / 12.f);

            return;
        }

        // Standard single bullet group
        Bullets::instance().addBulletGroup(getPosition(), aimAngle);
    }
}


void PlayerShip::setInvincibility(const bool isInvincible)
{
    this->isInvincible = isInvincible;

    if (this->isInvincible)
    {
        // Set the scale a bit smaller so when drawing the invincible sprite, it will cover it completely
        ship.setScale({0.9f, 0.9f});
        ship.setOrigin({
            Art::instance().invincibleBuff.getSize().x / 2.f,
            Art::instance().invincibleBuff.getSize().y / 2.f
        });
    }
    else
    {
        ship.setScale({1.f, 1.f});
        ship.setOrigin({
            Art::instance().player.getSize().x / 2.f,
            Art::instance().player.getSize().y / 2.f
        });
    }
}


void PlayerShip::setUsingBoosters(const bool isUsingBoosters)
{
    this->isUsingBoosters = isUsingBoosters;

    if (this->isUsingBoosters)
    {
        speed = BASE_SPEED * 1.5f;
        ship.setTexture(Art::instance().playerBoosters);
        ship.setOrigin({
            Art::instance().playerBoosters.getSize().x / 2.f,
            Art::instance().playerBoosters.getSize().y / 2.f
        });
    }
    else
    {
        speed = BASE_SPEED;
        ship.setTexture(Art::instance().player);
        ship.setOrigin({
            Art::instance().player.getSize().x / 2.f,
            Art::instance().player.getSize().y / 2.f
        });
    }
}


void PlayerShip::setUsingShotgunBullets(const bool isUsingShotgunBullets)
{
    this->isUsingShotgunBullets = isUsingShotgunBullets;
}


void PlayerShip::setUsingBulletsAllDirections(const bool isUsingBulletsAllDirections)
{
    this->isUsingBulletsAllDirections = isUsingBulletsAllDirections;
}


void PlayerShip::update()
{
    if (PlayerStatus::instance().isDead())
        return;

    // Move the player and clamp to window bounds
    velocity += speed * Input::instance().getMovementDirection();
    const sf::Vector2f nextPosition = ship.getPosition() + velocity;
    float clampedX = std::clamp(nextPosition.x, halfWidth(), GameRoot::instance().windowSizeF.x - halfWidth());
    float clampedY = std::clamp(nextPosition.y, halfHeight(), GameRoot::instance().windowSizeF.y - halfHeight());
    ship.setPosition({clampedX, clampedY});
    invincibleShip.setPosition(ship.getPosition());

    // Rotation the player ship in the direction of its velocity
    if (velocity.lengthSquared() > 0)
    {
        ship.setRotation(sf::radians(Extensions::toAngle(velocity)));
        invincibleShip.setRotation(ship.getRotation());
    }

    // Create the ship trail before resetting velocity
    createShipExhaust();
    createBulletGroups();

    // Make sure velocity always gets reset
    velocity = {0.0, 0.0};
}


void PlayerShip::draw() const
{
    if (!PlayerStatus::instance().isDead())
    {
        GaussianBlur::instance().drawToBase(ship);

        if (isInvincible)
            GaussianBlur::instance().drawToBase(invincibleShip);
    }
}

