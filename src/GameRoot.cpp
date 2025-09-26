#include <string>
#include <chrono>
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
#include "Logger/Logger.h"
#include "Particles/Particles.h"
#include "PlayerStatus/PlayerStatus.h"
#include "SFML/System/Sleep.hpp"
#include "UserInterface/Include/FloatingKillTexts.h"
#include "UserInterface/Include/UserInterface.h"


GameRoot::GameRoot()
{
    // Set fullscreen mode and set the window maximum size to it
    const auto& fullscreenModes = sf::VideoMode::getFullscreenModes();
    const unsigned int width = fullscreenModes[0].size.x;
    const unsigned int height = fullscreenModes[0].size.y;
    sf::Vector2 maxWindowSize {width, height};
    const unsigned int bitsPerPixel = fullscreenModes[0].bitsPerPixel;

    // Create the render window with fullscreen, 60 fps, and properties
    renderWindow = sf::RenderWindow(
        sf::VideoMode(maxWindowSize, bitsPerPixel),
        "Shape Wars",
        sf::Style::Default,
        sf::State::Fullscreen
    );
    renderWindow.setMaximumSize(maxWindowSize);
    renderWindow.setVerticalSyncEnabled(true);
    renderWindow.setMouseCursorVisible(false);

    // Set the screen size in float for easy maths
    windowSizeF = {static_cast<float>(width), static_cast<float>(height)};
    windowRectangle = {{0.0, 0.0}, {windowSizeF.x, windowSizeF.y}};

    Logger::printOut(
        "Created fullscreen window with size " + std::to_string(width) +
        " x " + std::to_string(height) + " and " +
        std::to_string(bitsPerPixel) + " bits per pixel"
    );
}


void GameRoot::toggleVsync()
{
    vsyncEnabled = !vsyncEnabled;
    renderWindow.setVerticalSyncEnabled(vsyncEnabled);
}


void GameRoot::togglePause()
{
    isPaused = !isPaused;

    // Note: Time marches on. Every running clock needs to be paused here (besides the delta and game clocks)
    if (isPaused)
    {
        PlayerStatus::instance().stopRoundClock();
    }
    else
    {
        PlayerStatus::instance().startRoundClock();
    }
}


std::chrono::milliseconds GameRoot::getCurrentTime()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch());
}


void GameRoot::run()
{
    // TODO: Play the game play song when the actual round starts???
    //       Create some ambient noise to play in the main menu until the game starts
    //       This will move to update soon
    Sound::instance().playGamePlaySong();

    // Main game loop, running at a fixed timestep of 60 fps
    while (renderWindow.isOpen())
    {
        elapsedGameTime = gameClock.getElapsedTime().asSeconds();
        deltaTime = deltaClock.restart().asSeconds();

        const auto start = getCurrentTime();

        processInput();
        update();
        render();

        sf::sleep(start + MS_PER_FRAME - getCurrentTime());
    }
}


void GameRoot::processInput()
{
    while (const std::optional event = renderWindow.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            renderWindow.close();

        // if (event->is<sf::Event::FocusLost>())
        //     isPaused = true;

        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            switch (keyPressed->scancode)
            {
            case sf::Keyboard::Scancode::Escape:
                renderWindow.close();
                break;

            case sf::Keyboard::Scancode::K:
                PlayerStatus::instance().markForKill();
                break;

            case sf::Keyboard::Scancode::P:
                togglePause();
                break;

            case sf::Keyboard::Scancode::O:
                Sound::instance().togglePlaySounds();
                break;

            case sf::Keyboard::Scancode::B:
                GaussianBlur::instance().toggleGaussianBlur();
                break;

            case sf::Keyboard::Scancode::V:
                toggleVsync();
                break;

                // Just to get clion to stop complaining about missing cases
            default:
                break;
            }

        }

        // Button pressed events
        if (const auto* joystickButtonPressed = event->getIf<sf::Event::JoystickButtonPressed>())
        {
            if (joystickButtonPressed->button == 6)
                renderWindow.close();

            if (joystickButtonPressed->button == 7)
                togglePause();
        }
    }

    // NOTE: Key pressed and button pushed events should eventually just be passed the input handler to be processed
    Input::instance().updateMousePosition();
}


void GameRoot::update()
{
    if (!isPaused)
    {
        // Always update the player status first
        PlayerStatus::instance().update();

        // When the player is alive
        if (!PlayerStatus::instance().isDead())
        {
            Enemies::instance().update();
            PlayerShip::instance().update();
            Bullets::instance().update();
            //BlackHoles::instance().update();
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
            PlayerShip::instance().centerPlayer();
            PlayerStatus::instance().needTotalReset = false;
        }

        // Independent of player status
        FloatingKillTexts::instance().update();
        Particles::instance().update();
        Grid::instance().update();
    }
}


void GameRoot::render()
{
    renderWindow.clear();

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

    renderWindow.display();
}
