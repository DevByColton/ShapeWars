#include <string>
#include <chrono>
#include "GameRoot.h"
#include "Content/Include/GaussianBlur.h"
#include "Content/Include/Sound.h"
#include "Entities/Include/Collisions.h"
#include "Systems/Include/Grid.h"
#include "Input/Include/Input.h"
#include "Core/Include/Logger.h"
#include "Entities/Include/Player/Buffs.h"
#include "Entities/Include/Player/PlayerStatus.h"
#include "GameState/Include/GamePlay.h"
#include "GameState/Include/PauseMenu.h"
#include "GameState/Include/StartMenu.h"
#include "SFML/Graphics/Image.hpp"
#include "SFML/System/Sleep.hpp"

GameRoot::GameRoot()
{
    // Set fullscreen mode and set the window maximum size to it
    const auto& fullscreenModes = sf::VideoMode::getFullscreenModes();
    const unsigned int width = fullscreenModes[0].size.x;
    const unsigned int height = fullscreenModes[0].size.y;
    sf::Vector2 maxWindowSize {width, height};
    const unsigned int bitsPerPixel = fullscreenModes[0].bitsPerPixel;

    // Create the render window with fullscreen and properties
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 8;
    renderWindow = sf::RenderWindow(
        sf::VideoMode(maxWindowSize, bitsPerPixel),
        "Shape Wars",
        sf::Style::Default,
        sf::State::Fullscreen,
        settings
    );
    renderWindow.setMaximumSize(maxWindowSize);
    renderWindow.setVerticalSyncEnabled(true);
    renderWindow.setMouseCursorVisible(false);
    renderWindow.setIcon(sf::Image("Content\\Images\\BlackHole.png"));

    // Set the screen size in float for easy maths
    windowSizeF = {static_cast<float>(width), static_cast<float>(height)};
    windowRectangle = {{0.0, 0.0}, {windowSizeF.x, windowSizeF.y}};
    topLeftCorner = {windowRectangle.position.x, windowRectangle.position.y};
    topRightCorner = {windowRectangle.size.x, windowRectangle.position.y};
    bottomRightCorner = {windowRectangle.size.x, windowRectangle.size.y};
    bottomLeftCorner = {windowRectangle.position.x, windowRectangle.size.y};

    // Set the minimum dead zone for controllers
    renderWindow.setJoystickThreshold(Input::instance().thumbStickDeadZone);

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


std::chrono::milliseconds GameRoot::getCurrentTime()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch());
}


bool GameRoot::isCurrentGameState(const GameState gameState) const
{
    return currentGameState == gameState;
}


void GameRoot::setCurrentGameState(const GameState nextGameState)
{
    currentGameState = nextGameState;
}


void GameRoot::run()
{
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
    // Event polling and handling
    while (const std::optional event = renderWindow.pollEvent())
    {
        // if (event->is<sf::Event::FocusLost>())
        //     isPaused = true;

        if (event->is<sf::Event::Closed>())
            renderWindow.close();

        // Set input mode to mouse and keyboard if the mouse moves
        if (event->is<sf::Event::MouseMoved>())
            Input::instance().inputMode = InputMode::MouseAndKeyboard;

        // Keyboard pressed events (these are hold events)
        if (event->is<sf::Event::KeyPressed>())
            Input::instance().inputMode = InputMode::MouseAndKeyboard;

        // Joystick buttons pressed events (these are hold events)
        if (event->is<sf::Event::JoystickButtonPressed>())
            Input::instance().inputMode = InputMode::Joystick;

        // Joystick axis connected event
        if (event->is<sf::Event::JoystickConnected>())
            Input::instance().checkConnectedDevice();

        if (const auto* mouseReleased = event->getIf<sf::Event::MouseButtonReleased>())
        {
            Input::instance().inputMode = InputMode::MouseAndKeyboard;

            switch (currentGameState)
            {
            case InStartMenu:
                StartMenu::instance().processMouseReleased(mouseReleased);
                break;
            case InPauseMenu:
                PauseMenu::instance().processMouseReleased(mouseReleased);
                break;
            case InGamePlay:
                GamePlay::instance().processMouseReleased(mouseReleased);
                break;
            }
        }

        // Keyboard released events (these are single press events that trigger on release)
        if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>())
        {
            Input::instance().inputMode = InputMode::MouseAndKeyboard;

            switch (currentGameState)
            {
            case InStartMenu:
                StartMenu::instance().processKeyReleased(keyReleased);
                break;
            case InPauseMenu:
                PauseMenu::instance().processKeyReleased(keyReleased);
                break;
            case InGamePlay:
                GamePlay::instance().processKeyReleased(keyReleased);
                break;
            }

            // Todo: Stuff below will be deleted eventually, move to settings pages

            if (keyReleased->scancode == sf::Keyboard::Scancode::Escape)
                renderWindow.close();

            if (keyReleased->scancode == sf::Keyboard::Scancode::O)
                Sound::instance().togglePlaySounds();

            if (keyReleased->scancode == sf::Keyboard::Scancode::B)
                GaussianBlur::instance().toggleGaussianBlur();

            if (keyReleased->scancode == sf::Keyboard::Scancode::V)
                toggleVsync();
        }

        // Joystick buttons released events (these are single press events that trigger on release)
        if (const auto* joystickButtonReleased = event->getIf<sf::Event::JoystickButtonReleased>())
        {
            Input::instance().inputMode = InputMode::Joystick;

            switch (currentGameState)
            {
            case InStartMenu:
                StartMenu::instance().processJoystickButtonReleased(joystickButtonReleased);
                break;
            case InPauseMenu:
                PauseMenu::instance().processJoystickButtonReleased(joystickButtonReleased);
                break;
            case InGamePlay:
                GamePlay::instance().processJoystickButtonReleased(joystickButtonReleased);
                break;
            }
        }

        // Joystick axis moved event
        if (const auto* joystickMoved = event->getIf<sf::Event::JoystickMoved>())
        {
            Input::instance().inputMode = InputMode::Joystick;

            switch (currentGameState)
            {
            case InStartMenu:
                StartMenu::instance().processJoystickAxisMoved(joystickMoved);
                break;
            case InPauseMenu:
                PauseMenu::instance().processJoystickAxisMoved(joystickMoved);
                break;
            case InGamePlay:
                GamePlay::instance().processJoystickAxisMoved(joystickMoved);
                break;
            }
        }
    }

    Input::instance().update();
}


void GameRoot::update() const
{
    switch (currentGameState)
    {
    case InStartMenu:
        StartMenu::instance().update();
        break;
    case InPauseMenu:
        PauseMenu::instance().update();
        break;
    case InGamePlay:
        GamePlay::instance().update();
        break;
    }
}


void GameRoot::render()
{
    // Clear the screen
    renderWindow.clear();

    if (currentGameState == InStartMenu)
    {
        StartMenu::instance().renderGaussianBlur();
        StartMenu::instance().renderToScreen();
    }
    else
    {
        GamePlay::instance().renderGaussianBlur();
        GamePlay::instance().renderToScreen();

        if (isCurrentGameState(InPauseMenu))
            PauseMenu::instance().renderToScreen();
    }

    Input::instance().draw();

    // Draw everything to the screen
    renderWindow.display();
}
