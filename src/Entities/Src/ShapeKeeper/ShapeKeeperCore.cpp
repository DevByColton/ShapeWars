#include "../../Include/ShapeKeeper/ShapeKeeperCore.h"
#include "../../../Content/Include/GaussianBlur.h"
#include "../../../Grid/Grid.h"
#include "../../../Particles/Particles.h"
#include "../../../System/Include/Extensions.h"
#include "../../../System/Include/RandomVector.h"
#include "../../Include/ShapeKeeper/ShapeKeeper.h"


ShapeKeeperCore::ShapeKeeperCore(const sf::Texture& texture, const sf::Color& color, ShapeKeeperHealthBar* healthBar) : Sprite(texture)
{
    this->color = color;
    this->healthBar = healthBar;
    radius = getTexture().getSize().x / 2.f - 7.f;
    setOrigin({getTexture().getSize().x / 2.f, getTexture().getSize().y / 2.f});
    trailSprite.setOrigin(getOrigin());

    reset();

    // Set the shield properties
    shieldRadius = shield.getTexture().getSize().x / 2.f - 10.f;
    shield.setOrigin({
        shield.getTexture().getSize().x / 2.f,
        shield.getTexture().getSize().y / 2.f
    });
    shield.setPosition(getPosition());
    shield.setRotation(getRotation());
}


void ShapeKeeperCore::reset()
{
    setPosition(offScreenLeft);
    setRotation(sf::radians(sf::degrees(324.f).asRadians()));
    trailSprite.setPosition(getPosition());
    trailSprite.setRotation(getRotation());
    shield.setPosition(getPosition());
    shield.setRotation(getRotation());
    wasHit = false;
    hasBeenHitByNuke = false;
    health = MAX_HEALTH;
    lastHitAmount = 0;
    timeUntilMovementChangeDuration = 0.f;
    timeUntilMovementChange = 0.f;
    previousTargetPosition = {0.f, 0.f};
    currentTargetPosition = {0.f, 0.f};
    currentBodyPartRotation = sf::Angle::Zero;
    previousRotationTarget = currentBodyPartRotation;
    currentRotationTarget = currentBodyPartRotation;
    spriteTrail.reset();
}


bool ShapeKeeperCore::isAlive() const
{
    return health > 0;
}


void ShapeKeeperCore::markForHit(const sf::Vector2f &hitPosition, const int amount)
{
    if (!wasHit && isAlive())
    {
        wasHit = true;
        lastHitAmount = amount;
        lastHitPosition = hitPosition;
    }
}


void ShapeKeeperCore::activate(const bool isOnLeftSideOfScreen)
{
    reset();

    if (isOnLeftSideOfScreen)
        setPosition(offScreenLeft);
    else
        setPosition(offScreenRight);

    // Manually set the first position and time to control how quickly the boss comes onto the screen
    // The first target is the middle of the screen from the side opposite the player
    timeUntilMovementChangeDuration = 5.f;
    timeUntilMovementChange = 0.f;
    previousTargetPosition = getPosition();
    currentTargetPosition = {GameRoot::instance().windowSizeF.x / 2.f, GameRoot::instance().windowSizeF.y / 2.f};
}


void ShapeKeeperCore::update()
{
    if (!isAlive())
        return;

    updateHealth();
    updateMovement();
    updateRotation();
    spriteTrail.update();
}


void ShapeKeeperCore::updateHealth()
{
    if (wasHit && isAlive())
    {
        // On each hit, spray some particles
        for (int i = 0; i < 50 * lastHitAmount; i++)
            Particles::instance().create(
                1.f,
                DontIgnoreGravity,
                i % 6 == 0 ? Explosion : Spark,
                lastHitPosition,
                RandomVector::instance().next(1.f, 11.f + lastHitAmount),
                color
            );

        // Reset hit variables and take damage
        health -= lastHitAmount;
        wasHit = false;
        lastHitAmount = 0;
    }

    // Update the health bar width to reflect health left
    healthBar->update(health, MAX_HEALTH);

    // Check death
    if (!isAlive())
    {
        Grid::instance().applyExplosiveForce(getPosition(), 400.f, 200.f, 0.8f);

        for (int i = 0; i < 2000; i++)
            Particles::instance().create(
                3.f,
                DontIgnoreGravity,
                i % 6 == 0 ? Explosion : Spark,
                getPosition(),
                RandomVector::instance().next(12.f, 64.f),
                color
            );

        // Function to emit core death
        onDeath();
    }
}


void ShapeKeeperCore::updateMovement()
{
    // Increment for position ease
    timeUntilMovementChange += GameRoot::instance().deltaTime;

    // Ease position
    setPosition(Extensions::easeInOutBack(previousTargetPosition, currentTargetPosition, timeUntilMovementChange / timeUntilMovementChangeDuration));
    shield.setPosition(getPosition());
    trailSprite.setPosition(getPosition());

    // Reset when elapsed
    if (timeUntilMovementChange > timeUntilMovementChangeDuration)
    {
        timeUntilMovementChangeDuration = timeUntilMovementChangeDistribution(timeUntilMovementChangeRandEngine);
        timeUntilMovementChange = 0.f;
        previousTargetPosition = getPosition();
        currentTargetPosition = {movementXDistribution(movementXRandEngine), movementYDistribution(movementYRandEngine)};
    }
}


void ShapeKeeperCore::updateRotation()
{
    timeUntilRotationChange += GameRoot::instance().deltaTime;

    // Ease rotation
    currentBodyPartRotation = sf::radians(Extensions::easeInOutSine(previousRotationTarget.asRadians(), currentRotationTarget.asRadians(), timeUntilRotationChange / timeUntilRotationChangeDuration));

    // Reset when needed
    if (timeUntilRotationChange > timeUntilRotationChangeDuration)
    {
        timeUntilRotationChange = 0.f;
        timeUntilRotationChangeDuration = timeUntilRotationChangeDistribution(timeUntilRotationChangeRandEngine);
        previousRotationTarget = currentBodyPartRotation;
        currentRotationTarget = sf::radians(rotationChangeDistribution(rotationChangeRandEngine));
    }
}


void ShapeKeeperCore::draw(const bool canTakeCoreDamage)
{
    if (!isAlive())
        return;

    spriteTrail.draw();

    GaussianBlur::instance().drawToBase(*this);

    if (!canTakeCoreDamage)
        GaussianBlur::instance().drawToBase(shield);
}
