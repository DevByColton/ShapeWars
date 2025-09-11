#include <iostream>
#include <SFML/Graphics.hpp>
#include "GameRoot.h"
#include "Content/Include/GaussianBlur.h"
#include "Content/Include/Sound.h"
#include "Entities/Include/BlackHoles.h"
#include "Entities/Include/Bullets.h"
#include "Entities/Include/Collisions.h"
#include "Entities/Include/Enemies.h"
#include "Entities/Include/PlayerShip.h"
#include "Grid/Grid.h"
#include "Input/Input.h"
#include "Particles/Particles.h"
#include "PlayerStatus/PlayerStatus.h"
#include "UserInterface/Include/UserInterface.h"
#include "UserInterface/Include/FloatingKillTexts.h"

int main()
{
    // Main game loop
    while (GameRoot::instance().renderWindow.isOpen()) {

        while (const std::optional event = GameRoot::instance().renderWindow.pollEvent()) {

            if (event->is<sf::Event::Closed>())
                GameRoot::instance().renderWindow.close();

            // if (event->is<sf::Event::FocusLost>())
            //     GameRoot::instance().isPaused = true;

            // NOTE: Key pressed events, I could pass these events to the input handler to process in update
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {

                switch (keyPressed->scancode)
                {
                    case sf::Keyboard::Scancode::Escape:
                        GameRoot::instance().renderWindow.close();
                        break;

                    case sf::Keyboard::Scancode::K:
                        PlayerStatus::instance().markForKill();
                        break;

                    case sf::Keyboard::Scancode::P:
                        GameRoot::instance().togglePause();
                        break;

                    case sf::Keyboard::Scancode::O:
                        Sound::instance().togglePlaySounds();
                        break;

                    case sf::Keyboard::Scancode::B:
                        GaussianBlur::instance().toggleGaussianBlur();
                        break;

                    // Just to get clion to stop complaining about missing cases
                    default:
                        break;
                }

            }

            // NOTE: I could pass these events to the input handler to process in update
            // Button pressed events
            if (const auto* joystickButtonPressed = event->getIf<sf::Event::JoystickButtonPressed>()) {

                if (joystickButtonPressed->button == 6)
                    GameRoot::instance().renderWindow.close();

                if (joystickButtonPressed->button == 7)
                    GameRoot::instance().togglePause();
            }

        }

        // Play the game play song when the game start
        Sound::instance().playGamePlaySong();

        // Updates independent of pause
        GameRoot::instance().update();
        Input::instance().updateMousePosition();

        // Pauseable updates
        if (!GameRoot::instance().isPaused) {

            // Always update the player status first
            PlayerStatus::instance().update();

            // When the player is alive
            if (!PlayerStatus::instance().isDead())
            {
                Enemies::instance().update();
                PlayerShip::instance().update();
                Bullets::instance().update();
                BlackHoles::instance().update();
                Collisions::instance().handleEnemyPlayerBullets();
                Collisions::instance().handleBlackHoles();
            }

            // At a minimum reset the enemies, black holes, and bullets
            if (PlayerStatus::instance().needBaseReset)
            {
                Bullets::instance().resetAll();
                Enemies::instance().killAll();
                BlackHoles::instance().killAll();
                PlayerStatus::instance().needBaseReset = false;
            }

            // At the restart of a new round, reset the player status, game clock, and center the player
            if (PlayerStatus::instance().needTotalReset)
            {
                PlayerStatus::instance().reset();
                GameRoot::instance().restartTotalGameClock();
                PlayerShip::instance().centerPlayer();
                PlayerStatus::instance().needTotalReset = false;
            }

            // Independent of player status
            FloatingKillTexts::instance().update();
            Particles::instance().update();
            Grid::instance().update();
        }

        // Draws
        GameRoot::instance().renderWindow.clear();

        // Draw stuff with bloom
        GaussianBlur::instance().clearTextures();
        Grid::instance().draw();
        Particles::instance().draw();
        Enemies::instance().draw();
        BlackHoles::instance().draw();
        Bullets::instance().draw();
        PlayerShip::instance().draw();
        GaussianBlur::instance().drawToScreen();

        // Draws without bloom
        FloatingKillTexts::instance().draw();
        Input::instance().draw();
        UserInterface::instance().draw();

        GameRoot::instance().renderWindow.display();

    }

    return 0;
}
