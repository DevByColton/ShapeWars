#include "../../Include/ShapeKeeper/ShapeKeeperBodyPart.h"
#include "../../../Content/Include/GaussianBlur.h"
#include "../../../Grid/Grid.h"
#include "../../../Particles/Particles.h"
#include "../../../System/Include/Extensions.h"
#include "../../../System/Include/RandomVector.h"


ShapeKeeperBodyPart::ShapeKeeperBodyPart(const sf::Texture& texture, const sf::Color& color,
                                         ShapeKeeperHealthBar* healthBar, ShapeKeeperCore* core,
                                         const float coreXOffset, const float coreYOffset) : Sprite(texture)
{
    this->coreXOffset = coreXOffset;
    this->coreYOffset = coreYOffset;
    this->color = color;
    this->healthBar = healthBar;
    this->core = core;
    radius = getTexture().getSize().x / 2.f - 10.f;
    setOrigin({radius, getTexture().getSize().y / 2.f});
    updatePositionRelativeToCore();
}


bool ShapeKeeperBodyPart::isAlive() const
{
    return health > 0;
}


void ShapeKeeperBodyPart::markForHit(const sf::Vector2f &hitPosition, const int amount)
{
    if (!wasHit && isAlive())
    {
        wasHit = true;
        lastHitAmount = amount;
        lastHitPosition = hitPosition;
    }
}


void ShapeKeeperBodyPart::updatePositionRelativeToCore()
{
    positionRelativeToCore = {core->getPosition().x + coreXOffset, core->getPosition().y + coreYOffset};
}


void ShapeKeeperBodyPart::updateHealth()
{
    if (wasHit && isAlive())
    {
        // On each hit, spray some particles
        for (int i = 0; i < 50 * lastHitAmount; i++)
            Particles::instance().create(
                1.f,
                DontIgnoreGravity,
                i % 4 == 0 ? Explosion : Spark,
                lastHitPosition,
                RandomVector::instance().next(1.f, 11.f + lastHitAmount),
                color
            );

        // Reset hit variables and take damage
        health -= lastHitAmount;
        wasHit = false;
        lastHitAmount = 0;

        // Update the health bar width to reflect health left
        healthBar->update(health, MAX_HEALTH);

        // Check death
        if (!isAlive())
        {
            Grid::instance().applyExplosiveForce(getPosition(), 400.f, 200.f, 0.8f);

            for (int i = 0; i < 750; i++)
                Particles::instance().create(
                    3.f,
                    DontIgnoreGravity,
                    i % 6 == 0 ? Explosion : Spark,
                    getPosition(),
                    RandomVector::instance().next(12.f, 48.f),
                    color
                );
        }
    }
}


void ShapeKeeperBodyPart::updateDirectionalAttack(const float t)
{
    sf::Vector2f direction = core->getPosition() - getPosition();

    if (direction.lengthSquared() > 1.f)
        direction = direction.normalized();

    direction.x *= directionalAttackAmount.x;
    direction.y *= directionalAttackAmount.y;

    setPosition(Extensions::lerp( positionRelativeToCore, positionRelativeToCore + direction, t));
}


void ShapeKeeperBodyPart::update()
{
    // Update health and life
    updateHealth();
    updatePositionRelativeToCore();
    setPosition(positionRelativeToCore);

    // Todo: this will change in updates
    // Maybe that's why attack methods can return is vectors of new positions???
    // setPosition(positionRelativeToCore);
}


void ShapeKeeperBodyPart::draw() const
{
    if (isAlive())
        GaussianBlur::instance().drawToBase(*this);
}
