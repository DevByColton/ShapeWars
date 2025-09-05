#include "../Include/BlackHoles.h"
#include "../../Content/Include/Bloom.h"
#include "../../Logger/Logger.h"
#include "../../PlayerStatus/PlayerStatus.h"
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
    float scaleX = 1 + 0.1f * std::cos(4 * GameRoot::instance().totalGameTime());
    float scaleY = 1 + 0.1f * std::sin(4 * GameRoot::instance().totalGameTime());
    sprite.setScale({scaleX, scaleY});
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
        Bloom::instance().drawToBaseBloomTexture(sprite);
}

