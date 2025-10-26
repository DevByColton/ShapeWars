#include "../Include/GamePlay.h"
#include "../../Core/Include/Logger.h"
#include "../../Entities/Include/BlackHoles.h"
#include "../../Entities/Include/Bullets.h"
#include "../../Entities/Include/Collisions.h"
#include "../../Entities/Include/Enemies.h"
#include "../../Entities/Include/Nukes.h"
#include "../../Entities/Include/Player/Buffs.h"
#include "../../Entities/Include/Player/PlayerShip.h"
#include "../../Entities/Include/Player/PlayerStatus.h"
#include "../../Entities/Include/ShapeKeeper/ShapeKeeper.h"
#include "../../Input/Include/Buttons.h"
#include "../../Input/Include/Input.h"
#include "../../Systems/Include/Grid.h"
#include "../../Systems/Include/Particles.h"
#include "../UI/Include/FloatingKillTexts.h"
#include "../UI/Include/LivesAndNukes.h"


GamePlay::GamePlay()
{
    ShapeKeeper::instance().currentGamePlayState = &currentGamePlayState;
}


void GamePlay::doBaseReset()
{
    Buffs::instance().resetBuffDrops();
    Bullets::instance().resetAll();
    Enemies::instance().killAll();
    BlackHoles::instance().killAll();
    Nukes::instance().reset();
    Nukes::instance().resetEnemiesSpawnTimer();
    PlayerStatus::instance().needBaseReset = false;
}


void GamePlay::doTotalReset()
{
    Buffs::instance().resetBuffs();
    PlayerStatus::instance().reset();
    PlayerShip::instance().reset();
    Nukes::instance().resetNukeCount();
    Enemies::instance().resetSpawnRates();
    BlackHoles::instance().resetSpawnRate();
    GamePlayHUD::instance().resetObjective();
    PlayerStatus::instance().needTotalReset = false;
    ShapeKeeper::instance().isDefeated = false;
    currentGamePlayState = PreBoss;
}


void GamePlay::startRound()
{
    markRoundStart = true;
}


void GamePlay::endRound()
{
    markRoundEnd = true;
}


void GamePlay::processKeyReleased(const sf::Event::KeyReleased* keyReleased)
{
    if (keyReleased->scancode == sf::Keyboard::Scancode::K)
    {
        PlayerStatus::instance().markForKill();
        return;
    }

    if (keyReleased->scancode == sf::Keyboard::Scancode::P)
    {
        GameRoot::instance().setCurrentGameState(InPauseMenu);
        PlayerStatus::instance().stopRoundClock();
        return;
    }

    if (keyReleased->scancode == sf::Keyboard::Scancode::Num1)
    {
        Buffs::instance().useBuff(1);
        return;
    }

    if (keyReleased->scancode == sf::Keyboard::Scancode::Num2)
    {
        Buffs::instance().useBuff(2);
        return;
    }

    if (keyReleased->scancode == sf::Keyboard::Scancode::Num3)
    {
        Buffs::instance().useBuff(3);
        return;
    }

    if (keyReleased->scancode == sf::Keyboard::Scancode::Num4)
    {
        Buffs::instance().useBuff(4);
        return;
    }

    if (keyReleased->scancode == sf::Keyboard::Scancode::Space)
    {
        Nukes::instance().markDetonate(PlayerShip::instance().getPosition());
        return;
    }

    if (keyReleased->scancode == sf::Keyboard::Scancode::E)
        ShapeKeeper::instance().endEncounter();
}


void GamePlay::processJoystickButtonReleased(const sf::Event::JoystickButtonReleased* joystickButtonReleased)
{
    if (Input::instance().isStartButton(joystickButtonReleased))
    {
        GameRoot::instance().setCurrentGameState(InPauseMenu);
        PlayerStatus::instance().stopRoundClock();
        return;
    }

    if (!PlayerStatus::instance().isDead())
    {
        if (Input::instance().isPrimaryButton(joystickButtonReleased))
        {
            Buffs::instance().useBuff(1);
            return;
        }

        if (Input::instance().isSecondaryButton(joystickButtonReleased))
        {
            Buffs::instance().useBuff(2);
            return;
        }

        if (Input::instance().isTertiaryButton(joystickButtonReleased))
        {
            Buffs::instance().useBuff(3);
            return;
        }

        // Last one so no need to return
        if (Input::instance().isQuaternaryButton(joystickButtonReleased))
            Buffs::instance().useBuff(4);
    }
}


void GamePlay::processJoystickAxisMoved(const sf::Event::JoystickMoved* joystickMoved)
{
    if (Input::instance().isAxisRightTrigger(joystickMoved) && Input::instance().wasRightTriggerReleased(joystickMoved))
        Nukes::instance().markDetonate(PlayerShip::instance().getPosition());
}


void GamePlay::update()
{
    // Check when the score area needs to transition in
    if (markRoundStart && !GamePlayHUD::instance().isTransitioningScoreAreaIn)
    {
        GamePlayHUD::instance().isTransitioningScoreAreaIn = true;
        markRoundStart = false;
    }

    // Check when the score area or health area needs to transition out
    if (markRoundEnd && !GamePlayHUD::instance().isTransitioningScoreAreaOut)
    {
        // Check if the shape keeper killed the player
        if (ShapeKeeper::instance().isActive && !ShapeKeeper::instance().isDefeated)
        {
            GamePlayHUD::instance().markHealthAreaTransitionOut(false);
            ShapeKeeper::instance().markDeactivate();
        }
        else
            GamePlayHUD::instance().isTransitioningScoreAreaOut = true;

        markRoundEnd = false;
    }

    // Always update the player status first
    PlayerStatus::instance().update();

    // Check when to activate the boss
    if (currentGamePlayState == PreBoss)
        ShapeKeeper::instance().checkActivate();

    // Make sure the boss gets updated during the fight
    if (currentGamePlayState == BossFight)
        ShapeKeeper::instance().update();

    // Core updates
    if (!PlayerStatus::instance().isDead())
    {
        Nukes::instance().update();
        Enemies::instance().update();
        Buffs::instance().update();
        PlayerShip::instance().update();
        Bullets::instance().update();
        BlackHoles::instance().update();
        Collisions::instance().handleEnemyPlayerBullets();
        Collisions::instance().handleBlackHoles();
        Collisions::instance().handlePlayerAndBuffs();
    }

    // Update boss collisions after core updates
    if (currentGamePlayState == BossFight)
        Collisions::instance().handleShapeKeeper();

    // When the player dies during a round
    if (PlayerStatus::instance().needBaseReset)
        doBaseReset();

    // At the re-start of a new round
    if (PlayerStatus::instance().needTotalReset)
    {
        doTotalReset();
        GameRoot::instance().setCurrentGameState(InStartMenu);
    }

    // Independent of player status
    FloatingKillTexts::instance().update();
    Particles::instance().update();
    Grid::instance().update();
    GamePlayHUD::instance().update();
}


void GamePlay::renderGaussianBlur()
{
    GaussianBlur::instance().clearTextures();
    Grid::instance().draw();
    Particles::instance().draw();
    Nukes::instance().draw();
    Enemies::instance().draw();
    BlackHoles::instance().draw();
    Bullets::instance().draw();
    Buffs::instance().draw();

    if (currentGamePlayState == BossFight)
        ShapeKeeper::instance().draw();

    PlayerShip::instance().draw();
    GamePlayHUD::instance().healthContainer.draw();
    GaussianBlur::instance().drawToScreen();
}

void GamePlay::renderToScreen()
{
    LivesAndNukes::instance().draw();
    FloatingKillTexts::instance().draw();
    Buffs::instance().drawText();
    Buttons::instance().draw();
    GamePlayHUD::instance().drawToScreen();
    GamePlayHUD::instance().healthContainer.drawText();
}
