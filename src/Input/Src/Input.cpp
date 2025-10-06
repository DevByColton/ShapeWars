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
    // See if a joystick is connected and supported, if so set the input mode to joystick
    for (int i = 0; i < sf::Joystick::Count; i++)
        if (sf::Joystick::isConnected(i))
        {
            const auto identification = sf::Joystick::getIdentification(i);
            if (Xbox::instance().isSupported(identification) ||
                Dualsense::instance().isSupported(identification))
                inputMode = InputMode::Joystick;
        }
}


bool Input::isMouseVisible() const
{
    return inputMode == InputMode::MouseAndKeyboard;
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


bool Input::isPrimaryButton(const sf::Event::JoystickButtonReleased* joystickButtonReleased)
{
    return Xbox::instance().isAButton(joystickButtonReleased) ||
           Dualsense::instance().isXButton(joystickButtonReleased);
}


bool Input::isSecondaryButton(const sf::Event::JoystickButtonReleased* joystickButtonReleased)
{
    return Xbox::instance().isBButton(joystickButtonReleased) ||
           Dualsense::instance().isCircleButton(joystickButtonReleased);
}


bool Input::isTertiaryButton(const sf::Event::JoystickButtonReleased* joystickButtonReleased)
{
    return Xbox::instance().isXButton(joystickButtonReleased) ||
           Dualsense::instance().isSquareButton(joystickButtonReleased);
}


bool Input::isQuaternaryButton(const sf::Event::JoystickButtonReleased* joystickButtonReleased)
{
    return Xbox::instance().isYButton(joystickButtonReleased) ||
           Dualsense::instance().isTriangleButton(joystickButtonReleased);
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

    // Sum the aiming direction by input mode
    switch (inputMode)
    {
    case InputMode::MouseAndKeyboard:
        direction += MouseAndKeyboard::instance().keysAimDirection();
        direction += MouseAndKeyboard::instance().mouseAimDirection(fromPosition);
        break;
    case InputMode::Joystick:
        for (int i = 0; i < sf::Joystick::Count; i++)
            if (sf::Joystick::isConnected(i))
            {
                const auto identification = sf::Joystick::getIdentification(i);
                direction += Xbox::instance().rightThumbStickPosition(i, identification);
                direction += Dualsense::instance().rightThumbStickPosition(i, identification);
            }
        break;
    }

    // Normalize so it does not matter which input the direction comes from
    if (direction.lengthSquared() > 1.f)
        direction = direction.normalized();

    return direction;
}

void Input::update()
{
    // Update the mouse sprite position to the actual mouse
    MouseAndKeyboard::instance().update();

    // Check for valid joystick inputs, if none set the input mode to mouse and keyboard
    hasValidXboxInput = false;
    hasValidDualsenseInput = false;

    for (int i = 0; i < sf::Joystick::Count; i++)
        if (sf::Joystick::isConnected(i))
        {
            const auto identification = sf::Joystick::getIdentification(i);

            if (Xbox::instance().isSupported(identification))
                hasValidXboxInput = true;

            if (Dualsense::instance().isSupported(identification))
                hasValidDualsenseInput = true;
        }

    if (!hasValidXboxInput && !hasValidDualsenseInput)
        inputMode = InputMode::MouseAndKeyboard;
}


void Input::draw() const
{
    if (isMouseVisible())
        MouseAndKeyboard::instance().draw();
}


