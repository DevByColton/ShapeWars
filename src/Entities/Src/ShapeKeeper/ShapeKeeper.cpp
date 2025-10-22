#include "../../Include/ShapeKeeper/ShapeKeeper.h"
#include "../../Include/Enemies.h"
#include "../../Include/Player/PlayerShip.h"
#include "../../Include/Player/PlayerStatus.h"


void ShapeKeeper::startEncounter()
{
    if (isActive)
        return;

    // Trigger health container transitions
    if (!healthContainer.isTransitioningIn)
        healthContainer.isTransitioningIn = true;

    // Spawn the boss opposite of the player, by default the boss is on the left side and reset to make sure all parts are in sync
    core.activate(PlayerShip::instance().getPosition().x > GameRoot::instance().windowSizeF.x / 2.f);
    core.onDeath = [this]
    {
        endEncounter();
        PlayerStatus::instance().addPoints(50'000);
    };
    laserBeams.reset();
    top.reset();
    middleLeft.reset();
    middleRight.reset();
    bottomLeft.reset();
    bottomRight.reset();

    // Enemies spawn will be triggered randomly
    Enemies::instance().canSpawn = false;

    isActive = true;
}


void ShapeKeeper::endEncounter()
{
    if (!isActive)
        return;

    // Trigger health container transitions
    if (!healthContainer.isTransitioningOut)
        healthContainer.isTransitioningOut = true;

    // Make enemies keep spawning for endless mode, black holes can spawn again
    timeUntilEnemiesSpawnElapsed = DEFAULT_TIME_UNTIL_ENEMIES_SPAWN;
    enemiesSpawningElapsed = 0.f;
    Enemies::instance().canSpawn = true;

    isActive = false;
}


bool ShapeKeeper::canTakeCoreDamage() const
{
    return !top.isAlive() && !middleLeft.isAlive() && !middleRight.isAlive() && !bottomLeft.isAlive() && !bottomRight.isAlive();
}


void ShapeKeeper::update()
{
    healthContainer.transitionIn();
    healthContainer.transitionOut();

    if (!isActive)
        return;

    // Update each part first
    core.update();
    laserBeams.update();
    top.update();
    middleLeft.update();
    middleRight.update();
    bottomLeft.update();
    bottomRight.update();

    //updateEnemiesSpawn();
}


void ShapeKeeper::updateEnemiesSpawn()
{
    if (!isActive)
        return;

    // Update enemies spawning trigger
    if (timeUntilEnemiesSpawnElapsed > 0.f)
    {
        timeUntilEnemiesSpawnElapsed -= GameRoot::instance().deltaTime;

        if (timeUntilEnemiesSpawnElapsed < 0.f)
        {
            Enemies::instance().canSpawn = true;
            enemiesSpawningElapsed = enemiesSpawningTimeDistribution(enemiesSpawningTimeEngine);
        }
    }

    // Update enemies spawn time
    if (enemiesSpawningElapsed > 0.f)
    {
        enemiesSpawningElapsed -= GameRoot::instance().deltaTime;

        if (enemiesSpawningElapsed < 0.f)
        {
            Enemies::instance().canSpawn = false;
            timeUntilEnemiesSpawnElapsed = timeUntilEnemiesSpawnDistribution(timeUntilEnemiesSpawnEngine);
        }
    }
}


void ShapeKeeper::draw()
{
    if (isActive)
    {
        laserBeams.draw();
        core.draw(canTakeCoreDamage());
        top.draw();
        middleLeft.draw();
        middleRight.draw();
        bottomLeft.draw();
        bottomRight.draw();
    }

    healthContainer.draw();
}


void ShapeKeeper::drawText()
{
    healthContainer.drawText();
}

