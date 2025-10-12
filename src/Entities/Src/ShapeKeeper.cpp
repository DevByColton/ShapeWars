#include "../Include/ShapeKeeper.h"
#include "../../Content/Include/GaussianBlur.h"
#include "../../Grid/Grid.h"
#include "../../Particles/Particles.h"
#include "../../System/Include/Extensions.h"
#include "../../System/Include/RandomVector.h"


ShapeKeeper::ShapeKeeperCore::ShapeKeeperCore(const sf::Texture& texture, const sf::Color& color) : Sprite(texture)
{
    this->color = color;
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


ShapeKeeper::ShapeKeeperBodyPart::ShapeKeeperBodyPart(const sf::Texture& texture, const sf::Color& color, const float coreXOffset, const float coreYOffset) : Sprite(texture)
{
    this->coreXOffset = coreXOffset;
    this->coreYOffset = coreYOffset;
    this->color = color;
    radius = getTexture().getSize().x / 2.f - 10.f;
    setOrigin({radius, getTexture().getSize().y / 2.f});
}


ShapeKeeper::ShapeKeeperHealthBar::ShapeKeeperHealthBar(const sf::Texture& texture, const sf::Vector2f &position) : Sprite(texture), background(texture)
{
    setPosition(position);
    setOrigin({getTexture().getSize().x / 2.f, getTexture().getSize().y / 2.f});
    background.setPosition(getPosition());
    background.setOrigin(getOrigin());
    background.setColor({getColor().r, getColor().g, getColor().b, 64});
}


ShapeKeeper::ShapeKeeper()
{
    // Wrapper for all the health bars and text
    healthWrapper.setSmooth(true);
    healthWrapperSprite.setPosition({GameRoot::instance().windowSizeF.x / 2.f, 50.f});
    healthWrapperSprite.setOrigin({
        healthWrapper.getSize().x / 2.f,
        healthWrapper.getSize().y / 2.f
    });

    // Shape keeper header font
    shapeKeeperText.setOrigin({
        shapeKeeperText.getLocalBounds().size.x / 2.f,
        shapeKeeperText.getLocalBounds().size.y / 2.f
    });
    shapeKeeperText.setPosition({healthWrapperSprite.getOrigin().x, 100.f});

    // Link all the parts to the core and health bars
    top.core = &core;
    top.healthBar = &topHealthBar;

    middleLeft.core = &core;
    middleLeft.healthBar = &middleLeftHealthBar;

    middleRight.core = &core;
    middleRight.healthBar = &middleRightHealthBar;

    bottomLeft.core = &core;
    bottomLeft.healthBar = &bottomLeftHealthBar;

    bottomRight.core = &core;
    bottomRight.healthBar = &bottomRightHealthBar;

    core.healthBar = &coreHealthBar;
}


bool ShapeKeeper::canTakeCoreDamage() const
{
    return !top.isAlive() && !middleLeft.isAlive() && !middleRight.isAlive() && !bottomLeft.isAlive() && !bottomRight.isAlive();
}


bool ShapeKeeper::ShapeKeeperBodyPart::isAlive() const
{
    return health > 0;
}


bool ShapeKeeper::ShapeKeeperCore::isAlive() const
{
    return health > 0;
}


void ShapeKeeper::ShapeKeeperCore::markForHit(const sf::Vector2f &hitPosition, const int amount)
{
    if (!wasHit && isAlive())
    {
        wasHit = true;
        lastHitAmount = amount;
        lastHitPosition = hitPosition;
    }
}


void ShapeKeeper::ShapeKeeperBodyPart::markForHit(const sf::Vector2f &hitPosition, const int amount)
{
    if (!wasHit && isAlive())
    {
        wasHit = true;
        lastHitAmount = amount;
        lastHitPosition = hitPosition;
    }
}


void ShapeKeeper::ShapeKeeperCore::update()
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
    healthBar->update(health);

    // Make sure the shield is always positioned on the core
    shield.setPosition(getPosition());
}


void ShapeKeeper::ShapeKeeperBodyPart::update()
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

    // Set the position relative to the core
    setPosition({core->getPosition().x + coreXOffset, core->getPosition().y + coreYOffset});

    // Update the health bar width to reflect health left
    healthBar->update(health);
}


void ShapeKeeper::ShapeKeeperHealthBar::update(const int health)
{
    sf::Vector2i size {0, static_cast<int>(getTexture().getSize().y)};

    if (health > 0)
    {
        // Normalize the health to the sprite width, or full width if health is full
        int x = static_cast<int>(getTexture().getSize().x);
        if (health != BODY_PART_MAX_HEALTH)
            x = health * getTexture().getSize().x / BODY_PART_MAX_HEALTH;

        // Set the health bar to reflect remaining health.
        // If the health is less 3 then do not reduce the width anymore,
        // It is basically not visible under 3 pixels so the body part could appear to be dead when it only
        // has a couple hit points left
        size.x = x < 3 ? 3 : x;
    }

    setTextureRect({{0,0}, size});
}


void ShapeKeeper::update()
{
    top.update();
    middleLeft.update();
    middleRight.update();
    bottomLeft.update();
    bottomRight.update();
    core.update();
}


void ShapeKeeper::draw()
{
    if (top.isAlive())
        GaussianBlur::instance().drawToBase(top);

    if (middleLeft.isAlive())
        GaussianBlur::instance().drawToBase(middleLeft);

    if (middleRight.isAlive())
        GaussianBlur::instance().drawToBase(middleRight);

    if (bottomLeft.isAlive())
        GaussianBlur::instance().drawToBase(bottomLeft);

    if (bottomRight.isAlive())
        GaussianBlur::instance().drawToBase(bottomRight);

    if (core.isAlive())
    {
        GaussianBlur::instance().drawToBase(core);

        if (!canTakeCoreDamage())
            GaussianBlur::instance().drawToBase(core.shield);
    }

    healthWrapper.clear(sf::Color::Transparent);
    healthWrapper.draw(shapeKeeperHealthFrame);
    healthWrapper.draw(topHealthBar.background);
    healthWrapper.draw(topHealthBar);
    healthWrapper.draw(middleLeftHealthBar.background);
    healthWrapper.draw(middleLeftHealthBar);
    healthWrapper.draw(middleRightHealthBar.background);
    healthWrapper.draw(middleRightHealthBar);
    healthWrapper.draw(bottomLeftHealthBar.background);
    healthWrapper.draw(bottomLeftHealthBar);
    healthWrapper.draw(bottomRightHealthBar.background);
    healthWrapper.draw(bottomRightHealthBar);
    healthWrapper.draw(coreHealthBar.background);
    healthWrapper.draw(coreHealthBar);
    healthWrapper.display();
    GaussianBlur::instance().drawToBase(healthWrapperSprite);
}


void ShapeKeeper::drawFont()
{
    shapeKeeperHealthFrame.setColor({255, 255, 255, static_cast<std::uint8_t>(255 * GameRoot::instance().frameUIOpacity)});

    healthWrapper.clear(sf::Color::Transparent);
    healthWrapper.draw(shapeKeeperText);
    healthWrapper.display();
    GameRoot::instance().renderWindow.draw(healthWrapperSprite);
}

