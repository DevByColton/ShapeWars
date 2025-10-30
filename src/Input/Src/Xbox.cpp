#include <algorithm>
#include <string>
#include "../Include/Xbox.h"
#include "../Include/Input.h"


bool Xbox::isSupported(const sf::Joystick::Identification& identification) const
{
    std::string controllerName = identification.name;
    std::ranges::transform(controllerName, controllerName.begin(), tolower);
    return controllerName.find(XBOX) != std::string::npos || controllerName.find(XINPUT) != std::string::npos;
}


bool Xbox::isDpadX(const sf::Event::JoystickMoved* joystickMoved) const
{
    if (!isSupported(sf::Joystick::getIdentification(joystickMoved->joystickId)))
        return false;

    return joystickMoved->axis == sf::Joystick::Axis::PovX;
}


bool Xbox::isDpadY(const sf::Event::JoystickMoved* joystickMoved) const
{
    if (!isSupported(sf::Joystick::getIdentification(joystickMoved->joystickId)))
        return false;

    return joystickMoved->axis == sf::Joystick::Axis::PovY;
}


bool Xbox::isLeftThumbstickX(const sf::Event::JoystickMoved* joystickMoved) const
{
    if (!isSupported(sf::Joystick::getIdentification(joystickMoved->joystickId)))
        return false;

    return joystickMoved->axis == sf::Joystick::Axis::X;
}


bool Xbox::isLeftThumbstickY(const sf::Event::JoystickMoved* joystickMoved) const
{
    if (!isSupported(sf::Joystick::getIdentification(joystickMoved->joystickId)))
        return false;

    return joystickMoved->axis == sf::Joystick::Axis::Y;
}


bool Xbox::isAxisRightThumbstick(const sf::Event::JoystickMoved* joystickMoved) const
{
    if (!isSupported(sf::Joystick::getIdentification(joystickMoved->joystickId)))
        return false;

    return joystickMoved->axis == sf::Joystick::Axis::U || joystickMoved->axis == sf::Joystick::Axis::V;
}


bool Xbox::isAxisRightTrigger(const sf::Event::JoystickMoved* joystickMoved) const
{
    if (!isSupported(sf::Joystick::getIdentification(joystickMoved->joystickId)))
        return false;

    return joystickMoved->axis == sf::Joystick::Axis::Z;
}


bool Xbox::wasRightTriggerReleased(const sf::Event::JoystickMoved* joystickMoved)
{
    if (!isSupported(sf::Joystick::getIdentification(joystickMoved->joystickId)))
        return false;

    previousRightTriggerPressed = currentRightTriggerPressed;
    currentRightTriggerPressed = joystickMoved->position < -Input::instance().triggerDeadZone;
    return previousRightTriggerPressed != currentRightTriggerPressed && !currentRightTriggerPressed;
}


bool Xbox::wasDpadMoved(const sf::Event::JoystickMoved* joystickMoved)
{
    if (!isSupported(sf::Joystick::getIdentification(joystickMoved->joystickId)))
        return false;

    const bool isInPositiveDeadZone = joystickMoved->position > Input::instance().thumbStickDeadZone;
    const bool isInNegativeDeadZone = joystickMoved->position < -Input::instance().thumbStickDeadZone;

    previousDpadMoved = currentDpadMoved;
    currentDpadMoved = isInPositiveDeadZone || isInNegativeDeadZone;
    return previousDpadMoved != currentDpadMoved && currentDpadMoved;
}


bool Xbox::wasLeftThumbstickMoved(const sf::Event::JoystickMoved* joystickMoved)
{
    if (!isSupported(sf::Joystick::getIdentification(joystickMoved->joystickId)))
        return false;

    const bool isInPositiveDeadZone = joystickMoved->position > Input::instance().thumbStickDeadZone;
    const bool isInNegativeDeadZone = joystickMoved->position < -Input::instance().thumbStickDeadZone;

    previousLeftThumbstickMoved = currentLeftThumbstickMoved;
    currentLeftThumbstickMoved = isInPositiveDeadZone || isInNegativeDeadZone;
    return previousLeftThumbstickMoved != currentLeftThumbstickMoved && currentLeftThumbstickMoved;
}


sf::Vector2f Xbox::leftThumbStickPosition(const int index, const sf::Joystick::Identification& identification) const
{
    if (!isSupported(identification))
        return {0.f, 0.f};

    return Input::instance().thumbStickPosition(index, sf::Joystick::Axis::X, sf::Joystick::Axis::Y);
}


sf::Vector2f Xbox::rightThumbStickPosition(const int index, const sf::Joystick::Identification& identification) const
{
    if (!isSupported(identification))
        return {0.f, 0.f};

    return Input::instance().thumbStickPosition(index, sf::Joystick::Axis::U, sf::Joystick::Axis::V);
}

bool Xbox::isBackButton(const sf::Event::JoystickButtonReleased* joystickButtonReleased) const
{
    if (!isSupported(sf::Joystick::getIdentification(joystickButtonReleased->joystickId)))
        return false;

    return joystickButtonReleased->button == 6;
}


bool Xbox::isStartButton(const sf::Event::JoystickButtonReleased* joystickButtonReleased) const
{
    if (!isSupported(sf::Joystick::getIdentification(joystickButtonReleased->joystickId)))
        return false;

    return joystickButtonReleased->button == 7;
}


bool Xbox::isAButton(const sf::Event::JoystickButtonReleased* joystickButtonReleased) const
{
    if (!isSupported(sf::Joystick::getIdentification(joystickButtonReleased->joystickId)))
        return false;

    return joystickButtonReleased->button == 0;
}


bool Xbox::isBButton(const sf::Event::JoystickButtonReleased* joystickButtonReleased) const
{
    if (!isSupported(sf::Joystick::getIdentification(joystickButtonReleased->joystickId)))
        return false;

    return joystickButtonReleased->button == 1;
}


bool Xbox::isXButton(const sf::Event::JoystickButtonReleased* joystickButtonReleased) const
{
    if (!isSupported(sf::Joystick::getIdentification(joystickButtonReleased->joystickId)))
        return false;

    return joystickButtonReleased->button == 2;
}


bool Xbox::isYButton(const sf::Event::JoystickButtonReleased* joystickButtonReleased) const
{
    if (!isSupported(sf::Joystick::getIdentification(joystickButtonReleased->joystickId)))
        return false;

    return joystickButtonReleased->button == 3;
}