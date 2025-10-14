#include <string>
#include <chrono>
#include "GameRoot.h"
#include "Content/Include/GaussianBlur.h"
#include "Content/Include/Sound.h"
#include "Entities/Include/BlackHoles.h"
#include "Entities/Include/Bullets.h"
#include "Entities/Include/Collisions.h"
#include "Entities/Include/Enemies.h"
#include "Entities/Include/Nukes.h"
#include "Entities/Include/ShapeKeeper/ShapeKeeper.h"
#include "Entities/Include/Player/PlayerShip.h"
#include "Grid/Grid.h"
#include "Input/Include/Input.h"
#include "System/Include/Logger.h"
#include "Particles/Particles.h"
#include "Entities/Include/Player/Buffs.h"
#include "Entities/Include/Player/PlayerStatus.h"
#include "SFML/Graphics/Image.hpp"
#include "SFML/System/Sleep.hpp"
#include "UserInterface/Include/Buttons.h"
#include "UserInterface/Include/FloatingKillTexts.h"
#include "UserInterface/Include/LivesAndNukes.h"
#include "UserInterface/Include/UserInterface.h"


GameRoot::GameRoot()
{
    // Set fullscreen mode and set the window maximum size to it
    const auto& fullscreenModes = sf::VideoMode::getFullscreenModes();
    const unsigned int width = fullscreenModes[0].size.x;
    const unsigned int height = fullscreenModes[0].size.y;
    sf::Vector2 maxWindowSize {width, height};
    const unsigned int bitsPerPixel = fullscreenModes[0].bitsPerPixel;

    sf::ContextSettings settings;
    settings.antiAliasingLevel = 8;

    // Create the render window with fullscreen and properties
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
    renderWindow.setJoystickThreshold(15.f);

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

        if (const auto* _ = event->getIf<sf::Event::MouseMoved>())
            Input::instance().inputMode = InputMode::MouseAndKeyboard;

        // Keyboard pressed events (these are hold events)
        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            processKeyPressed(keyPressed);

        // Keyboard released events (these are single press events that trigger on release)
        if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>())
            processKeyReleased(keyReleased);

        // Joystick buttons pressed events (these are hold events)
        if (const auto* joystickButtonPressed = event->getIf<sf::Event::JoystickButtonPressed>())
            processJoystickButtonPressed(joystickButtonPressed);

        // Joystick buttons released events (these are single press events that trigger on release)
        if (const auto* joystickButtonReleased = event->getIf<sf::Event::JoystickButtonReleased>())
            processJoystickButtonReleased(joystickButtonReleased);

        // Joystick axis moved event
        if (const auto* joystickMoved = event->getIf<sf::Event::JoystickMoved>())
            processJoystickAxisMoved(joystickMoved);

        // Joystick axis connected event
        if (const auto* _ = event->getIf<sf::Event::JoystickConnected>())
            Input::instance().checkConnectedDevice();
    }

    Input::instance().update();
}


void GameRoot::processKeyPressed(const sf::Event::KeyPressed* keyPressed)
{
    Input::instance().inputMode = InputMode::MouseAndKeyboard;
}


void GameRoot::processKeyReleased(const sf::Event::KeyReleased* keyReleased)
{
    Input::instance().inputMode = InputMode::MouseAndKeyboard;

    if (keyReleased->scancode == sf::Keyboard::Scancode::Escape)
    {
        renderWindow.close();
        return;
    }

    if (keyReleased->scancode == sf::Keyboard::Scancode::K)
    {
        PlayerStatus::instance().markForKill();
        return;
    }

    if (keyReleased->scancode == sf::Keyboard::Scancode::Space)
    {
        Nukes::instance().markDetonate(PlayerShip::instance().getPosition());
        return;
    }

    if (keyReleased->scancode == sf::Keyboard::Scancode::P)
    {
        togglePause();
        return;
    }

    if (keyReleased->scancode == sf::Keyboard::Scancode::O)
    {
        Sound::instance().togglePlaySounds();
        return;
    }

    if (keyReleased->scancode == sf::Keyboard::Scancode::B)
    {
        GaussianBlur::instance().toggleGaussianBlur();
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

    if (keyReleased->scancode == sf::Keyboard::Scancode::R)
        ShapeKeeper::instance().startEncounter();

    if (keyReleased->scancode == sf::Keyboard::Scancode::E)
        ShapeKeeper::instance().endEncounter();

    // if (keyReleased->scancode == sf::Keyboard::Scancode::Num7)
    //     ShapeKeeper::instance().triggerDirectionalAttack();

    // Last one, so no need to return
    if (keyReleased->scancode == sf::Keyboard::Scancode::V)
        toggleVsync();
}


void GameRoot::processJoystickButtonPressed(const sf::Event::JoystickButtonPressed* joystickButtonPressed)
{
    Input::instance().inputMode = InputMode::Joystick;

    // No hold events implemented currently
}


void GameRoot::processJoystickButtonReleased(const sf::Event::JoystickButtonReleased* joystickButtonReleased)
{
    Input::instance().inputMode = InputMode::Joystick;

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

        if (Input::instance().isQuaternaryButton(joystickButtonReleased))
        {
            Buffs::instance().useBuff(4);
            return;
        }
    }

    // Last one, so no need to return
    if (Input::instance().isStartButton(joystickButtonReleased))
    {
        togglePause();
        return;
    }

    if (Input::instance().isBackButton(joystickButtonReleased))
        renderWindow.close();
}


void GameRoot::processJoystickAxisMoved(const sf::Event::JoystickMoved* joystickMoved)
{
    Input::instance().inputMode = InputMode::Joystick;

    // Last one, so no need to return
    if (Input::instance().isAxisRightTrigger(joystickMoved) && Input::instance().wasRightTriggerReleased(joystickMoved))
        Nukes::instance().markDetonate(PlayerShip::instance().getPosition());
}


void GameRoot::update() const
{
    if (!isPaused)
    {
        // Always update the player status first
        PlayerStatus::instance().update();
        ShapeKeeper::instance().update();

        // When the player is alive
        if (!PlayerStatus::instance().isDead())
        {
            Nukes::instance().update();
            //Enemies::instance().update();
            Buffs::instance().update();
            PlayerShip::instance().update();
            Bullets::instance().update();
            //BlackHoles::instance().update();
            Collisions::instance().handleEnemyPlayerBullets();
            Collisions::instance().handleBlackHoles();
            Collisions::instance().handlePlayerAndBuffs();
            Collisions::instance().handlePlayerAndShapeKeeper();
        }

        // When the player dies during a round
        if (PlayerStatus::instance().needBaseReset)
        {
            Buffs::instance().resetBuffDrops();
            Bullets::instance().resetAll();
            Enemies::instance().killAll();
            BlackHoles::instance().killAll();
            Nukes::instance().reset();
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
}


void GameRoot::render()
{
    // Draw to the screen
    renderWindow.clear();

    // Draw stuff with bloom
    GaussianBlur::instance().clearTextures();
    Grid::instance().draw();
    Particles::instance().draw();
    Nukes::instance().draw();
    Enemies::instance().draw();
    BlackHoles::instance().draw();
    Bullets::instance().draw();
    ShapeKeeper::instance().draw();
    PlayerShip::instance().draw();
    Buffs::instance().draw();

    GaussianBlur::instance().drawToScreen();
    LivesAndNukes::instance().draw();
    FloatingKillTexts::instance().draw();
    UserInterface::instance().draw();
    Buffs::instance().drawText();
    Buttons::instance().draw();
    ShapeKeeper::instance().drawText();
    Input::instance().draw();

    renderWindow.display();
}
