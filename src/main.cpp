#include <SFML/Graphics.hpp>
#include "GameRoot.h"
#include "Entities/Include/Bullets.h"
#include "Entities/Include/PlayerShip.h"
#include "Input/Input.h"

int main()
{
    // Main game loop
    while (GameRoot::instance().renderWindow.isOpen()) {
        while (const std::optional event = GameRoot::instance().renderWindow.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                GameRoot::instance().renderWindow.close();

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                    GameRoot::instance().renderWindow.close();
        }

        // Updates
        Input::instance().update();
        PlayerShip::instance().update();
        Bullets::instance().update();

        // Draws
        GameRoot::instance().renderWindow.clear();
        Bullets::instance().draw();
        PlayerShip::instance().draw();
        Input::instance().draw();
        GameRoot::instance().renderWindow.display();
    }
}
