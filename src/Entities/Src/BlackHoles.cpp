#include <cmath>
#include <numbers>
#include "../Include/BlackHoles.h"
#include "../../Content/Include/GaussianBlur.h"
#include "../../Grid/Grid.h"
#include "../../Particles/Particles.h"
#include "../../PlayerStatus/PlayerStatus.h"
#include "../../System/Include/ColorPicker.h"
#include "../../System/Include/Extensions.h"
#include "../../System/Include/SpawnHelper.h"
#include "../../UserInterface/Include/FloatingKillTexts.h"

BlackHoles::BlackHoles()
{
    resetBlackHolePool();
}


void BlackHoles::resetBlackHolePool()
{
    // Set the head of the list
    firstAvailable = &blackHoles.at(0);

    // Create a linked list of black holes
    for (int i = 0; i < MAX_BLACK_HOLE_COUNT - 1; i++)
        blackHoles.at(i).setNext(&blackHoles.at(i + 1));

    // The last one in the list terminates
    blackHoles.at(MAX_BLACK_HOLE_COUNT - 1).setNext(nullptr);
}


BlackHoles::BlackHole* BlackHoles::BlackHole::getNext() const
{
    return next;
}


void BlackHoles::BlackHole::setNext(BlackHole* next)
{
    this->next = next;
}

void BlackHoles::BlackHole::activate()
{
    sprite.setPosition(SpawnHelper::instance().createSpawnPosition());
    hitPoints = 10;
    pointValue = 50;
    isActive = true;
}


void BlackHoles::spawnBlackHole()
{
    assert(firstAvailable != nullptr);

    // Make sure it is not the last in the list
    BlackHole* newBlackHole = firstAvailable;
    if (newBlackHole->getNext() != nullptr)
    {
        firstAvailable = newBlackHole->getNext();
        newBlackHole->activate();
    }
}


BlackHoles::BlackHole::BlackHole()
{
    // Set the radius and origin of the black holes
    float x = sprite.getTexture().getSize().x / 2;
    float y = sprite.getTexture().getSize().y / 2;
    sprite.setOrigin({x, y});
    radius = x;
}


void BlackHoles::BlackHole::reset()
{
    sprite.setPosition({0.0, 0.0});
    isActive = false;
    wasHit = false;
    particleSprayTime = maxParticleSprayTime;
}


bool BlackHoles::BlackHole::getWasHit() const
{
    return wasHit;
}


void BlackHoles::BlackHole::markHit(const bool wasKillShot)
{
    // Only mark was hit if not already hit this frame
    if (!wasHit)
    {
        wasHit = true;
        this->wasKillShot = wasKillShot;
    }
}


bool BlackHoles::BlackHole::hit()
{
    hitPoints -= wasKillShot ? hitPoints : 1;
    wasHit = false;

    std::uniform_real_distribution<float> particleStartOffset {0.f, PI / hitParticleCount};
    std::uniform_real_distribution<float> magnitude {8.f, 16.f};
    const float startOffset = particleStartOffset(instance().randEngine);

    for (int i = 0; i < hitParticleCount; i++)
    {
        const sf::Vector2f sprayVelocity = Extensions::fromPolar(PI * 2 * i / hitParticleCount + startOffset, magnitude(instance().randEngine));
        const sf::Vector2f position = getPosition() + 2.f * sprayVelocity;
        Particles::instance().create(
            3.f,
            IgnoreGravity,
            Explosion,
            position,
            sprayVelocity,
            i % 2 == 0 ? hitParticleColorPrimary : hitParticleColorSecondary
        );
    }

    if (hitPoints <= 0)
    {
        kill(true);
        return true;
    }

    return false;
}


void BlackHoles::BlackHole::kill(const bool addPoints)
{
    if (addPoints)
    {
        PlayerStatus::instance().addPoints(pointValue);
        PlayerStatus::instance().increaseMultiplier();
        FloatingKillTexts::instance().add(pointValue * PlayerStatus::instance().multiplier, sprite.getPosition());
    }

    reset();
}


sf::Vector2f BlackHoles::BlackHole::getPosition() const
{
    return sprite.getPosition();
}


void BlackHoles::killAll()
{
    for (int i = 0; i < MAX_BLACK_HOLE_COUNT; i++)
        if (blackHoles.at(i).isActive)
            blackHoles.at(i).kill(false);

    resetBlackHolePool();
}


void BlackHoles::resetAll()
{
    for (int i = 0; i < MAX_BLACK_HOLE_COUNT; i++)
        blackHoles.at(i).reset();
}


void BlackHoles::update()
{
    // Check spawn chance
    if (canSpawn && spawnDistribution(randEngine) == 0)
        spawnBlackHole();

    // Update all the black holes and check hit status
    // If the black hole dies, then set it to the front of the list
    for (int i = 0; i < MAX_BLACK_HOLE_COUNT; i++)
        if (blackHoles.at(i).isActive)
            if (blackHoles.at(i).getWasHit() && blackHoles.at(i).hit())
            {
                blackHoles.at(i).setNext(firstAvailable);
                firstAvailable = &blackHoles.at(i);
            }
            else
            {
                blackHoles.at(i).update();
            }
}


void BlackHoles::BlackHole::update()
{
    // Update the scale so the black holes pulse
    float scale = 1.f + 0.1f * std::sin(6.f * GameRoot::instance().elapsedGameTime);
    sprite.setScale({scale, scale});

    // Pull the grid in around the black hole, with some variation using sin
    Grid::instance().applyImplosiveForce(getPosition(), std::sin(particleSprayAngle / 2.f) * 5.f + 20.f, 200.f);

    // Spray some random particles
    particleSprayTime -= GameRoot::instance().deltaTime;
    if (particleSprayTime <= 0.f)
    {
        particleSprayTime = maxParticleSprayTime;
        const sf::Vector2f sprayVelocity = Extensions::fromPolar(particleSprayAngle, magnitude(instance().randEngine));
        const float sprayAngleValue = sprayAngle(instance().randEngine);
        sf::Vector2f position = getPosition() + 3.f * sf::Vector2f(sprayVelocity.y, -sprayVelocity.x);
        position.x += sprayAngleValue;
        position.y += sprayAngleValue;
        Particles::instance().create(
            3.f,
            DontIgnoreGravity,
            Explosion,
            position,
            sprayVelocity,
            particleSprayColor
        );
    }

    // Rotate the spray angle
    particleSprayAngle += TWO_PI * 0.1f;
    if (particleSprayAngle > TWO_PI)
        particleSprayAngle = 0.f;
}


void BlackHoles::draw() const
{
    // Draw the black holes pulsing
    for (int i = 0; i < MAX_BLACK_HOLE_COUNT; i++)
        blackHoles.at(i).draw();
}


void BlackHoles::BlackHole::draw() const
{
    if (isActive)
        GaussianBlur::instance().drawToBase(sprite);
}

