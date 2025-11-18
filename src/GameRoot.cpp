#include <string>
#include <chrono>
#include "GameRoot.h"
#include "Content/Include/GaussianBlur.h"
#include "Content/Include/Sound.h"
#include "Entities/Include/Collisions.h"
#include "Systems/Include/Grid.h"
#include "Input/Include/Input.h"
#include "Core/Include/Logger.h"
#include "Entities/Include/BlackHoles.h"
#include "Entities/Include/Enemies.h"
#include "Entities/Include/Nukes.h"
#include "Entities/Include/Player/Buffs.h"
#include "Entities/Include/Player/PlayerShip.h"
#include "Entities/Include/Player/PlayerStatus.h"
#include "Entities/Include/ShapeKeeper/ShapeKeeper.h"
#include "GameState/Include/GamePlay.h"
#include "GameState/Include/OptionsMenu.h"
#include "GameState/Include/PauseMenu.h"
#include "GameState/Include/StartMenu.h"
#include "GameState/UI/Include/FloatingKillTexts.h"
#include "Input/Include/Dualsense.h"
#include "Input/Include/MouseAndKeyboard.h"
#include "Input/Include/Xbox.h"
#include "SFML/Graphics/Image.hpp"
#include "SFML/System/Sleep.hpp"
#include "Systems/Include/Particles.h"

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


void GameRoot::setActiveInputState(IGameState* gameState)
{
    activeInputState = gameState;
}

void GameRoot::addUpdatableState(IGameState* gameState)
{
    for (int s = 0; s < MAX_GAME_STATE_COUNT; s++)
        if (updatableGameStates.at(s) == nullptr)
        {
            updatableGameStates.at(s) = gameState;
            break;
        }
}


void GameRoot::removeUpdatableState(const IGameState* gameState)
{
    int foundIndex = -1;

    for (int s = 0; s < MAX_GAME_STATE_COUNT; s++)
        if (updatableGameStates.at(s) == gameState)
        {
            foundIndex = s;
            break;
        }

    if (foundIndex != -1)
        updatableGameStates.at(foundIndex) = nullptr;
}


void GameRoot::addDrawableState(IGameState* gameState)
{
    for (int s = 0; s < MAX_GAME_STATE_COUNT; s++)
        if (drawableGameStates.at(s) == nullptr)
        {
            drawableGameStates.at(s) = gameState;
            break;
        }
}


void GameRoot::removeDrawableState(const IGameState* gameState)
{
    int foundIndex = -1;

    for (int s = 0; s < MAX_GAME_STATE_COUNT; s++)
        if (drawableGameStates.at(s) == gameState)
        {
            foundIndex = s;
            break;
        }

    if (foundIndex != -1)
        drawableGameStates.at(foundIndex) = nullptr;
}


std::chrono::milliseconds GameRoot::getCurrentTime()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch());
}


void GameRoot::load()
{
    // Make sure all singleton instances have been loaded, this will cause everything to load upfront
    Sound::instance();
    Art::instance();
    GaussianBlur::instance();
    Buffs::instance();
    PlayerStatus::instance();
    PlayerShip::instance();
    ShapeKeeper::instance();
    BlackHoles::instance();
    Enemies::instance();
    Bullets::instance();
    Nukes::instance();
    Collisions::instance();
    GamePlay::instance();
    OptionsMenu::instance();
    PauseMenu::instance();
    StartMenu::instance();
    FloatingKillTexts::instance();
    GamePlayHUD::instance();
    Input::instance();
    Xbox::instance();
    Dualsense::instance();
    MouseAndKeyboard::instance();
    Grid::instance();
    Particles::instance();

    // Set start menu as the first state
    StartMenu::instance().transitionTo();
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

        // Joystick buttons pressed events (these are hold events)
        if (event->is<sf::Event::JoystickButtonPressed>())
            Input::instance().inputMode = InputMode::Joystick;

        // Joystick axis connected event
        if (event->is<sf::Event::JoystickConnected>())
            Input::instance().checkConnectedDevice();

        // Mouse moved events
        if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>())
        {
            Input::instance().inputMode = InputMode::MouseAndKeyboard;
            activeInputState->processMouseMoved(mouseMoved);
        }

        // Mouse pressed events
        if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
        {
            Input::instance().inputMode = InputMode::MouseAndKeyboard;
            activeInputState->processMousePressed(mousePressed);
        }

        // Mouse release events
        if (const auto* mouseReleased = event->getIf<sf::Event::MouseButtonReleased>())
        {
            Input::instance().inputMode = InputMode::MouseAndKeyboard;
            activeInputState->processMouseReleased(mouseReleased);
        }

        // Mouse wheel scroll events
        if (const auto* mouseWheelScrolled = event->getIf<sf::Event::MouseWheelScrolled>())
        {
            Input::instance().inputMode = InputMode::MouseAndKeyboard;
            activeInputState->processMouseWheelScrolledEvent(mouseWheelScrolled);
        }

        // Keyboard pressed events (these are hold events)
        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            Input::instance().inputMode = InputMode::MouseAndKeyboard;
            activeInputState->processKeyPressed(keyPressed);
        }

        // Keyboard released events (these are single press events that trigger on release)
        if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>())
        {
            Input::instance().inputMode = InputMode::MouseAndKeyboard;
            activeInputState->processKeyReleased(keyReleased);

            // NOTE: For debugging
            // if (keyReleased->scancode == sf::Keyboard::Scancode::K)
            //     renderWindow.close();

            // if (keyReleased->scancode == sf::Keyboard::Scancode::B)
            //     GaussianBlur::instance().toggleGaussianBlur();
        }

        // Joystick buttons released events (these are single press events that trigger on release)
        if (const auto* joystickButtonReleased = event->getIf<sf::Event::JoystickButtonReleased>())
        {
            Input::instance().inputMode = InputMode::Joystick;
            activeInputState->processJoystickButtonReleased(joystickButtonReleased);
        }

        // Joystick axis moved event
        if (const auto* joystickMoved = event->getIf<sf::Event::JoystickMoved>())
        {
            Input::instance().inputMode = InputMode::Joystick;
            activeInputState->processJoystickAxisMoved(joystickMoved);
        }
    }

    Input::instance().update();
}


void GameRoot::update() const
{
    for (int s = 0; s < MAX_GAME_STATE_COUNT; s++)
        if (updatableGameStates.at(s) != nullptr)
            updatableGameStates.at(s)->update();

    // Always update sound
    Sound::instance().update();
}


void GameRoot::render()
{
    // Clear the screen
    renderWindow.clear();

    for (int s = 0; s < MAX_GAME_STATE_COUNT; s++)
        if (drawableGameStates.at(s) != nullptr)
        {
            drawableGameStates.at(s)->renderGaussianBlur();
            drawableGameStates.at(s)->renderToScreen();
        }

    Input::instance().draw();

    // Draw everything to the screen
    renderWindow.display();
}
