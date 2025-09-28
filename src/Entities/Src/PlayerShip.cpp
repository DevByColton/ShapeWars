#include <algorithm>
#include <cmath>
#include "../Include/PlayerShip.h"
#include "../../GameRoot.h"
#include "../../Content/Include/Art.h"
#include "../../Content/Include/GaussianBlur.h"
#include "../../Input/Input.h"
#include "../../Particles/Particles.h"
#include "../../PlayerStatus/PlayerStatus.h"
#include "../../System/Include/Extensions.h"
#include "../../System/Include/RandomVector.h"
#include "../Include/Bullets.h"

PlayerShip::PlayerShip()
{
    // Set the players position in the middle of the screen
    sprite.setPosition(GameRoot::instance().windowSizeF / 2.f);

    // Set the sprite origin to the middle of the sprite
    float spriteMiddleX = static_cast<float>(Art::instance().player.getSize().x / 2.0);
    float spriteMiddleY = static_cast<float>(Art::instance().player.getSize().y / 2.0);
    sprite.setOrigin({spriteMiddleX, spriteMiddleY});

    // Set the size of the sprite in float
    float width = static_cast<float>(sprite.getTexture().getSize().x);
    float height = static_cast<float>(sprite.getTexture().getSize().y);
    spriteSizeF = {width, height};
}


void PlayerShip::centerPlayer()
{
    sprite.setPosition(GameRoot::instance().windowSizeF / 2.f);
    sprite.setRotation(sf::Angle::Zero);
}


sf::Vector2f PlayerShip::getPosition() const
{
    return sprite.getPosition();
}


float PlayerShip::halfWidth() const
{
    return spriteSizeF.x / 2;
}


float PlayerShip::halfHeight() const
{
    return spriteSizeF.y / 2;
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
        const sf::Vector2f exhaustPosition = getPosition() + Extensions::transform({-25.f, 0.f}, quaternion);

        // Center particle stream
        const sf::Vector2f velMid = baseVelocity + RandomVector::instance().next(0.f, 1.f);
        Particles::instance().create(
            0.5f,
            DontIgnoreGravity,
            Spark,
            exhaustPosition,
            velMid,
            white,
            orangeYellow
        );

        // Side particle streams with glow
        Particles::instance().create(
            0.5f,
            DontIgnoreGravity,
            Spark,
            exhaustPosition,
            baseVelocity + perpendicularVelocity,
            white,
            deepRed
        );
        Particles::instance().create(
            0.5f,
            DontIgnoreGravity,
            Spark,
            exhaustPosition,
            baseVelocity - perpendicularVelocity,
            white,
            deepRed
        );
    }
}


void PlayerShip::update()
{
    // Move the player and clamp to window bounds
    velocity += speed * Input::instance().getMovementDirection();
    const sf::Vector2f nextPosition = sprite.getPosition() + velocity;
    float clampedX = std::clamp(nextPosition.x, halfWidth(), GameRoot::instance().windowSizeF.x - halfWidth());
    float clampedY = std::clamp(nextPosition.y, halfHeight(), GameRoot::instance().windowSizeF.y - halfHeight());
    sprite.setPosition({clampedX, clampedY});

    // Rotation the player ship in the direction of its velocity
    if (velocity.lengthSquared() > 0)
        sprite.setRotation(sf::radians(Extensions::toAngle(velocity)));

    // Check aiming direction and fire bullets
    const sf::Vector2f aimDirection = Input::instance().getAimDirection(getPosition());
    if (aimDirection.lengthSquared() > 0 && spawnBulletCooldownRemaining <= 0) {
        // Reset fire bullets cooldown
        spawnBulletCooldownRemaining = spawnBulletCooldownFrames;
        Bullets::instance().addBulletGroup(getPosition(), aimDirection);
    }

    // Create the ship trail before resetting velocity
    createShipExhaust();

    // Make sure velocity always gets reset
    velocity = {0.0, 0.0};

    // Decrement the bullet cooldown
    if (spawnBulletCooldownRemaining > 0)
        spawnBulletCooldownRemaining -= 1;
}


void PlayerShip::draw() const
{
    if (!PlayerStatus::instance().isDead())
        GaussianBlur::instance().drawToBase(sprite);
}

