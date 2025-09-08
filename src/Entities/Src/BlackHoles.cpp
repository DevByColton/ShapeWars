#include <cmath>
#include <numbers>
#include "../Include/BlackHoles.h"
#include "../../Content/Include/GaussianBlur.h"
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
    particleSprayClock.start();
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
    particleSprayClock.reset();
}


void BlackHoles::BlackHole::reset()
{
    sprite.setPosition({0.0, 0.0});
    isActive = false;
    wasHit = false;
}


bool BlackHoles::BlackHole::getWasHit() const
{
    return wasHit;
}


void BlackHoles::BlackHole::markHit()
{
    // Only mark was hit if not already hit this frame
    if (!wasHit)
        wasHit = true;
}


bool BlackHoles::BlackHole::hit()
{
    hitPoints -= 1;
    wasHit = false;

    constexpr int numParticles = 150;
    std::uniform_real_distribution<float> particleStartOffset {0.0f, std::numbers::pi / numParticles};
    std::uniform_real_distribution<float> magnitude {8.0f, 16.0f};
    const float hue = std::fmod(3.0f * GameRoot::instance().totalGameTimeSeconds(), 6.0f);
    const sf::Color color = ColorPicker::instance().hsvToRgb(hue, 0.25f, 1.0f);
    const float startOffset = particleStartOffset(instance().randEngine);

    for (int i = 0; i < numParticles; i++)
    {
        const sf::Vector2f sprayVelocity = fromPolar(std::numbers::pi * 2 * i / numParticles + startOffset, magnitude(instance().randEngine));
        const sf::Vector2f position = getPosition() + 2.0f * sprayVelocity;
        Particles::instance().create(
            GameRoot::instance().fps * 3,
            IgnoreGravity,
            Explosion,
            position,
            sprayVelocity,
            color
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
    if (spawnDistribution(randEngine) == 0)
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
    float scaleX = 1.0f + 0.1f * std::cos(6 * GameRoot::instance().totalGameTimeSeconds());
    float scaleY = 1.0f + 0.1f * std::sin(8 * GameRoot::instance().totalGameTimeSeconds());
    sprite.setScale({scaleX, scaleY});

    // Spray some random particles
    if (particleSprayClock.getElapsedTime() > particleSprayInterval)
    {
        particleSprayClock.restart();
        const sf::Vector2f sprayVelocity = fromPolar(particleSprayAngle, magnitude(instance().randEngine));
        const sf::Color lightPurple = ColorPicker::instance().hsvToRgb(5.2f, 0.75f, 0.8f);
        const float sprayAngleValue = sprayAngle(instance().randEngine);
        sf::Vector2f position = getPosition() + 2.0f * sf::Vector2f(sprayVelocity.y, -sprayVelocity.x);
        position.x += sprayAngleValue;
        position.y += sprayAngleValue;
        Particles::instance().create(
            GameRoot::instance().fps * 3,
            DontIgnoreGravity,
            Explosion,
            position,
            sprayVelocity,
            lightPurple
        );
    }

    // Rotate the spray angle
    particleSprayAngle -= std::numbers::pi * 2.0f / 75.0f;
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

