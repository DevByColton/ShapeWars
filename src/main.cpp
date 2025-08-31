#include <iostream>
#include <SFML/Graphics.hpp>
#include "GameRoot.h"
#include "Content/Sound.h"
#include "Entities/Include/Bullets.h"
#include "Entities/Include/Collisions.h"
#include "Entities/Include/Enemies.h"
#include "Entities/Include/PlayerShip.h"
#include "Input/Input.h"
#include "PlayerStatus/PlayerStatus.h"
#include "UserInterface/UserInterface.h"

int main()
{

    // long cpp_version = __cplusplus;
    // std::cout << "C++ Standard Version: " << cpp_version << std::endl;

    // Main game loop
    while (GameRoot::instance().renderWindow.isOpen()) {

        while (const std::optional event = GameRoot::instance().renderWindow.pollEvent()) {

            if (event->is<sf::Event::Closed>())
                GameRoot::instance().renderWindow.close();

            if (event->is<sf::Event::FocusLost>())
                GameRoot::instance().isPaused = true;

            // NOTE: I could pass these events to the input handler to process in update
            // Key pressed events
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {

                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                    GameRoot::instance().renderWindow.close();

                if (keyPressed->scancode == sf::Keyboard::Scancode::P)
                    GameRoot::instance().togglePause();
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
            Enemies::instance().update();
            PlayerShip::instance().update();
            PlayerStatus::instance().update();
            Bullets::instance().update();
            Collisions::instance().handleEnemyAndPlayer();

            // If the player died, reset all enemies and bullets
            if (PlayerShip::instance().isDead()) {
                Bullets::instance().resetAll();
                Enemies::instance().killAll();
            }
        }

        // Draws
        GameRoot::instance().renderWindow.clear();
        Enemies::instance().draw();
        Bullets::instance().draw();
        PlayerShip::instance().draw();
        Input::instance().draw();
        UserInterface::instance().draw();
        GameRoot::instance().renderWindow.display();

    }

    return 0;
}
