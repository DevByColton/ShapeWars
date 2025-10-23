#include "../Include/GamePlay.h"
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

    if (keyReleased->scancode == sf::Keyboard::Scancode::R)
        ShapeKeeper::instance().startEncounter();

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
    // Always update the player status first
    PlayerStatus::instance().update();
    ShapeKeeper::instance().update();

    // When the player is alive
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
        Collisions::instance().handleShapeKeeper();
    }

    // When the player dies during a round
    if (PlayerStatus::instance().needBaseReset)
    {
        Buffs::instance().resetBuffDrops();
        Bullets::instance().resetAll();
        Enemies::instance().killAll();
        BlackHoles::instance().killAll();
        Nukes::instance().reset();
        Nukes::instance().resetEnemiesSpawnTimer();
        PlayerStatus::instance().needBaseReset = false;
    }

    // At the restart of a new round
    if (PlayerStatus::instance().needTotalReset)
    {
        Buffs::instance().resetBuffs();
        PlayerStatus::instance().reset();
        PlayerShip::instance().reset();
        Nukes::instance().resetNukeCount();
        Enemies::instance().resetSpawnRates();
        BlackHoles::instance().resetSpawnRate();
        PlayerStatus::instance().needTotalReset = false;
    }

    // Independent of player status
    FloatingKillTexts::instance().update();
    Particles::instance().update();
    Grid::instance().update();
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
    ShapeKeeper::instance().draw();
    Buffs::instance().draw();
    PlayerShip::instance().draw();
    GaussianBlur::instance().drawToScreen();
}

void GamePlay::renderToScreen()
{
    LivesAndNukes::instance().draw();
    FloatingKillTexts::instance().draw();
    Buffs::instance().drawText();
    ShapeKeeper::instance().drawText();
    gamePlayHud.drawToScreen();

    //UserInterface::instance().draw();
    Buttons::instance().draw();
}
