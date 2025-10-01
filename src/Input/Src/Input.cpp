#include "../Include/Input.h"
#include "../Include/Dualsense.h"
#include "../Include/Xbox.h"
#include "../Include/MouseAndKeyboard.h"
#include "SFML/Window/Joystick.hpp"


Input::Input()
{
    checkConnectedDevice();
}


void Input::checkConnectedDevice()
{
    // See if a joystick is connected and supported, if so set the aim mode to controller
    for (int i = 0; i < sf::Joystick::Count; i++)
        if (sf::Joystick::isConnected(i))
        {
            const auto identification = sf::Joystick::getIdentification(i);
            if (Xbox::instance().isSupported(identification) ||
                Dualsense::instance().isSupported(identification))
                setAimMode(AimMode::Joystick);
        }
}


void Input::setAimMode(const AimMode aimMode)
{
    this->aimMode = aimMode;
}


bool Input::isMouseVisible() const
{
    return aimMode == AimMode::Mouse;
}


bool Input::isAxisRightThumbstick(const sf::Event::JoystickMoved* joystickMoved) const
{
    return Xbox::instance().isAxisRightThumbstick(joystickMoved) ||
           Dualsense::instance().isAxisRightThumbstick(joystickMoved);
}


bool Input::isAxisRightTrigger(const sf::Event::JoystickMoved* joystickMoved) const
{
    return Xbox::instance().isAxisRightTrigger(joystickMoved) ||
           Dualsense::instance().isAxisRightTrigger(joystickMoved);
}


bool Input::wasRightTriggerReleased(const sf::Event::JoystickMoved* joystickMoved)
{
    return Xbox::instance().wasRightTriggerReleased(joystickMoved) ||
           Dualsense::instance().wasRightTriggerReleased(joystickMoved);
}


bool Input::isBackButton(const sf::Event::JoystickButtonReleased* joystickButtonReleased)
{
    return Xbox::instance().isBackButton(joystickButtonReleased) ||
           Dualsense::instance().isBackButton(joystickButtonReleased);
}


bool Input::isStartButton(const sf::Event::JoystickButtonReleased* joystickButtonReleased)
{
    return Xbox::instance().isStartButton(joystickButtonReleased) ||
           Dualsense::instance().isStartButton(joystickButtonReleased);
}


void Input::mouseMoved()
{
    setAimMode(AimMode::Mouse);
}


sf::Vector2f Input::thumbStickPosition(const int index, const sf::Joystick::Axis thumbStickX, const sf::Joystick::Axis thumbStickY) const
{
    sf::Vector2f position {0.f, 0.f};

    // Get the positions of the thumbstick, x and y
    const float x = sf::Joystick::getAxisPosition(index, thumbStickX);
    const float y = sf::Joystick::getAxisPosition(index, thumbStickY);

    // Account for thumb stick dead zone
    const bool isXDeadZoneTolerant = x < -thumbStickDeadZone || x > thumbStickDeadZone;
    const bool isYDeadZoneTolerant = y < -thumbStickDeadZone || y > thumbStickDeadZone;

    // If is in dead zone tolerance
    if (isXDeadZoneTolerant || isYDeadZoneTolerant) {
        position.x = x;
        position.y = y;
    }

    // Vector is un-normalized. Returns a zero vector if the thumbstick is not above dead zone range
    return position;
}


sf::Vector2f Input::getMovementDirection() const
{
    sf::Vector2f direction {0.f, 0.f};

    // Sum mouse and keyboard movement if any
    direction += MouseAndKeyboard::instance().movementDirection();

    // Sum directions of each connected joystick if any
    for (int i = 0; i < sf::Joystick::Count; i++)
        if (sf::Joystick::isConnected(i))
        {
            const auto identification = sf::Joystick::getIdentification(i);
            direction += Xbox::instance().leftThumbStickPosition(i, identification);
            direction += Dualsense::instance().leftThumbStickPosition(i, identification);
        }

    // Normalize
    if (direction.lengthSquared() > 1.f)
        direction = direction.normalized();

    return direction;
}


sf::Vector2f Input::getAimDirection(const sf::Vector2f& fromPosition) const
{
    sf::Vector2f direction = {0.f, 0.f};

    switch (aimMode)
    {
        case AimMode::Keyboard:
            direction = MouseAndKeyboard::instance().keysAimDirection();
            break;
        case AimMode::Mouse:
            direction = MouseAndKeyboard::instance().mouseAimDirection(fromPosition);
            break;
        case AimMode::Joystick:
            // Sum directions of each connected joystick this will get the directions
            for (int i = 0; i < sf::Joystick::Count; i++)
                if (sf::Joystick::isConnected(i))
                {
                    const auto identification = sf::Joystick::getIdentification(i);
                    direction += Xbox::instance().rightThumbStickPosition(i, identification);
                    direction += Dualsense::instance().rightThumbStickPosition(i, identification);
                }
            break;
    }

    // Normalize
    if (direction.lengthSquared() > 1.f)
        direction = direction.normalized();

    return direction;
}

void Input::update()
{
    MouseAndKeyboard::instance().update();

    // If the aim mode is joystick, just make sure a controller is still connected
    if (aimMode == AimMode::Joystick)
    {
        bool isAtLeastOneSupportedJoystickConnected = false;

        for (int i = 0; i < sf::Joystick::Count; i++)
            if (sf::Joystick::isConnected(i))
            {
                const auto identification = sf::Joystick::getIdentification(i);
                if (Xbox::instance().isSupported(identification) ||
                    Dualsense::instance().isSupported(identification))
                {
                    isAtLeastOneSupportedJoystickConnected = true;
                    break;
                }
            }

        if (!isAtLeastOneSupportedJoystickConnected)
            aimMode = AimMode::Mouse;
    }
}


void Input::draw() const
{
    if (isMouseVisible())
        MouseAndKeyboard::instance().draw();
}


