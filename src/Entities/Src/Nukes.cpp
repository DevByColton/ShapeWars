#include "../Include/Nukes.h"
#include "../../Content/Include/GaussianBlur.h"
#include "../../Systems/Include/Grid.h"
#include "../Include/Player/PlayerStatus.h"
#include "../../Core/Include/Extensions.h"
#include "../Include/BlackHoles.h"
#include "../Include/Enemies.h"


Nukes::Nukes()
{
    nukeCircle.setOutlineThickness(2.5f);
    nukeCircle.setOutlineColor(nukeColor);
    nukeCircle.setFillColor(sf::Color::Transparent);
}


void Nukes::reset()
{
    radius = INITIAL_RADIUS;
    velocity = INITIAL_VELOCITY;
    nukeCircle.setRadius(radius);
    nukeCircle.setPosition({0.f, 0.f});
    isDetonating = false;
}


void Nukes::resetEnemiesSpawnTimer()
{
    timeUntilEnemiesSpawnAfterElapsed = 0.f;
}


void Nukes::resetNukeCount()
{
    count = MAX_NUKE_COUNT;
}


sf::Vector2f Nukes::getPosition() const
{
    return nukeCircle.getPosition();
}


void Nukes::markDetonate(const sf::Vector2f &fromPosition)
{
    if (!PlayerStatus::instance().isDead() && !isDetonating && count > 0)
    {
        isDetonating = true;
        count -= 1;
        startEnemiesSpawnTimer();
        nukeCircle.setPosition(fromPosition);
        Grid::instance().applyExplosiveForce(fromPosition, 400.f, 450.f, 0.9f);
    }
}


void Nukes::startEnemiesSpawnTimer()
{
    Enemies::instance().canSpawn = false;
    BlackHoles::instance().canSpawn = false;
    timeUntilEnemiesSpawnAfterElapsed = TIME_UNTIL_ENEMIES_SPAWN_AFTER_DETONATION_DURATION;
}


void Nukes::updateEnemiesSpawnTimer()
{
    if (timeUntilEnemiesSpawnAfterElapsed > 0.f)
    {
        timeUntilEnemiesSpawnAfterElapsed -= GameRoot::instance().deltaTime;

        if (timeUntilEnemiesSpawnAfterElapsed < 0.f)
        {
            Enemies::instance().canSpawn = true;
            BlackHoles::instance().canSpawn = true;
        }
    }
}


void Nukes::update()
{
    updateEnemiesSpawnTimer();

    if (isDetonating)
    {
        radius = nukeCircle.getRadius() + velocity;
        nukeCircle.setRadius(radius);
        nukeCircle.setOrigin(nukeCircle.getGeometricCenter());

        // When all four corners of the screen are contained that means the circle is bigger than the screen, reset
        const float radii = nukeCircle.getRadius() * nukeCircle.getRadius();
        const bool topLeftContained = Extensions::distanceSquared(nukeCircle.getPosition(), GameRoot::instance().topLeftCorner) <= radii;
        const bool topRightContained = Extensions::distanceSquared(nukeCircle.getPosition(), GameRoot::instance().topRightCorner) <= radii;
        const bool bottomRightContained = Extensions::distanceSquared(nukeCircle.getPosition(), GameRoot::instance().bottomRightCorner) <= radii;
        const bool bottomLeftContained = Extensions::distanceSquared(nukeCircle.getPosition(), GameRoot::instance().bottomLeftCorner) <= radii;
        if (topLeftContained && topRightContained && bottomRightContained && bottomLeftContained)
            reset();
    }
}


void Nukes::draw() const
{
    if (isDetonating)
        GaussianBlur::instance().drawToBase(nukeCircle);
}
