#include "../../Include/ShapeKeeper/ShapeKeeper.h"
#include "../../../Core/Include/Logger.h"
#include "../../../GameState/UI/Include/GamePlayHUD.h"
#include "../../Include/BlackHoles.h"
#include "../../Include/Enemies.h"
#include "../../Include/Player/PlayerShip.h"
#include "../../Include/Player/PlayerStatus.h"


void ShapeKeeper::checkActivate()
{
    if (!isActive && !isDefeated && PlayerStatus::instance().score > 50'000)
    {
        *currentGamePlayState = BossFight;
        GamePlayHUD::instance().markScoreAreaTransitionOut(true);
        startEncounter();
    }
}


void ShapeKeeper::checkDeactivate()
{
    if (timeUntilDeactivateElapsed > 0.f)
    {
        timeUntilDeactivateElapsed -= GameRoot::instance().deltaTime;

        if (timeUntilDeactivateElapsed < 0.f)
            endEncounter();
    }
}


void ShapeKeeper::startEncounter()
{
    if (isActive)
        return;

    // Spawn the boss opposite of the player, by default the boss is on the left side and reset to make sure all parts are in sync
    core.activate(PlayerShip::instance().getPosition().x > GameRoot::instance().windowSizeF.x / 2.f);
    core.onDeath = [this]
    {
        isDefeated = true;
        endEncounter();
        PlayerStatus::instance().addPoints(50'000);
        GamePlayHUD::instance().setObjectiveEndless();
        GamePlayHUD::instance().markHealthAreaTransitionOut(true);
        Enemies::instance().canSpawn = true;
    };
    laserBeams.reset();
    top.reset();
    middleLeft.reset();
    middleRight.reset();
    bottomLeft.reset();
    bottomRight.reset();

    // Enemies spawn will be triggered randomly
    Enemies::instance().canSpawn = false;

    isDefeated = false;
    isActive = true;
}


void ShapeKeeper::endEncounter()
{
    if (!isActive)
        return;

    // Make enemies keep spawning for endless mode
    timeUntilEnemiesSpawnElapsed = DEFAULT_TIME_UNTIL_ENEMIES_SPAWN;
    enemiesSpawningElapsed = 0.f;

    if (isDefeated)
        *currentGamePlayState = Endless;

    isActive = false;
}


void ShapeKeeper::markDeactivate()
{
    timeUntilDeactivateElapsed = TIME_UNTIL_DEACTIVATE_DURATION;
    core.deactivate();
}


bool ShapeKeeper::canTakeCoreDamage() const
{
    return !top.isAlive() && !middleLeft.isAlive() && !middleRight.isAlive() && !bottomLeft.isAlive() && !bottomRight.isAlive();
}


void ShapeKeeper::update()
{
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

    updateEnemiesSpawn();
    checkDeactivate();
}


void ShapeKeeper::updateEnemiesSpawn()
{
    if (!isActive && !PlayerStatus::instance().isGameOver())
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
    if (!isActive)
        return;

    laserBeams.draw();
    core.draw(canTakeCoreDamage());
    top.draw();
    middleLeft.draw();
    middleRight.draw();
    bottomLeft.draw();
    bottomRight.draw();
}

