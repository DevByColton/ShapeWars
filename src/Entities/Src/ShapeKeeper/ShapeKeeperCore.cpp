#include "../../Include/ShapeKeeper/ShapeKeeperCore.h"
#include "../../../Content/Include/GaussianBlur.h"
#include "../../../Grid/Grid.h"
#include "../../../Particles/Particles.h"
#include "../../../System/Include/RandomVector.h"
#include "../../Include/ShapeKeeper/ShapeKeeper.h"


ShapeKeeperCore::ShapeKeeperCore(const sf::Texture& texture, const sf::Color& color, ShapeKeeperHealthBar* healthBar) : Sprite(texture)
{
    this->color = color;
    this->healthBar = healthBar;
    radius = getTexture().getSize().x / 2.f - 7.f;
    setOrigin({getTexture().getSize().x / 2.f, getTexture().getSize().y / 2.f});

    setPosition({
        GameRoot::instance().windowSizeF.x / 2.f,
        GameRoot::instance().windowSizeF.y / 2.f
    });

    // Start the core rotated
    setRotation(sf::radians(sf::degrees(324.f).asRadians()));

    // Set the shield properties
    shieldRadius = shield.getTexture().getSize().x / 2.f - 10.f;
    shield.setOrigin({
        shield.getTexture().getSize().x / 2.f,
        shield.getTexture().getSize().y / 2.f
    });
    shield.setPosition(getPosition());
    shield.setRotation(getRotation());
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


void ShapeKeeperCore::update()
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
        }
    }

    // Update the health bar width to reflect health left
    healthBar->update(health, MAX_HEALTH);

    // Make sure the shield is always positioned on the core
    shield.setPosition(getPosition());
}


void ShapeKeeperCore::draw(const bool canTakeCoreDamage) const
{
    if (isAlive())
    {
        GaussianBlur::instance().drawToBase(*this);

        if (!canTakeCoreDamage)
            GaussianBlur::instance().drawToBase(shield);
    }
}
