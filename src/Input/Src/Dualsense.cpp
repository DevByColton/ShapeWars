#include <algorithm>
#include <string>
#include "../Include/Dualsense.h"
#include "../Include/Input.h"


bool Dualsense::isSupported(const sf::Joystick::Identification& identification) const
{
    std::string controllerName = identification.name;
    std::ranges::transform(controllerName, controllerName.begin(), tolower);
    return controllerName.find(PS4) != std::string::npos ||
           controllerName.find(PS5) != std::string::npos ||
           controllerName.find(PLAYSTATION) != std::string::npos ||
           controllerName.find(DUALSENSE) != std::string::npos;
}


bool Dualsense::isDpadX(const sf::Event::JoystickMoved* joystickMoved) const
{
    if (!isSupported(sf::Joystick::getIdentification(joystickMoved->joystickId)))
        return false;

    return joystickMoved->axis == sf::Joystick::Axis::PovX;
}


bool Dualsense::isDpadY(const sf::Event::JoystickMoved* joystickMoved) const
{
    if (!isSupported(sf::Joystick::getIdentification(joystickMoved->joystickId)))
        return false;

    return joystickMoved->axis == sf::Joystick::Axis::PovY;
}


bool Dualsense::isLeftThumbstickX(const sf::Event::JoystickMoved* joystickMoved) const
{
    if (!isSupported(sf::Joystick::getIdentification(joystickMoved->joystickId)))
        return false;

    return joystickMoved->axis == sf::Joystick::Axis::X;
}


bool Dualsense::isLeftThumbstickY(const sf::Event::JoystickMoved* joystickMoved) const
{
    if (!isSupported(sf::Joystick::getIdentification(joystickMoved->joystickId)))
        return false;

    return joystickMoved->axis == sf::Joystick::Axis::Y;
}


bool Dualsense::isAxisRightThumbstick(const sf::Event::JoystickMoved* joystickMoved) const
{
    if (!isSupported(sf::Joystick::getIdentification(joystickMoved->joystickId)))
        return false;

    return joystickMoved->axis == sf::Joystick::Axis::Z || joystickMoved->axis == sf::Joystick::Axis::R;
}


bool Dualsense::isAxisRightTrigger(const sf::Event::JoystickMoved* joystickMoved) const
{
    if (!isSupported(sf::Joystick::getIdentification(joystickMoved->joystickId)))
        return false;

    return joystickMoved->axis == sf::Joystick::Axis::V;
}


bool Dualsense::wasRightTriggerReleased(const sf::Event::JoystickMoved* joystickMoved)
{
    if (!isSupported(sf::Joystick::getIdentification(joystickMoved->joystickId)))
        return false;

    previousRightTriggerPressed = currentRightTriggerPressed;
    currentRightTriggerPressed = joystickMoved->position > 100 - Input::instance().triggerDeadZone;
    return previousRightTriggerPressed != currentRightTriggerPressed && !currentRightTriggerPressed;
}


bool Dualsense::wasLeftThumbstickMoved(const sf::Event::JoystickMoved* joystickMoved)
{
    if (!isSupported(sf::Joystick::getIdentification(joystickMoved->joystickId)))
        return false;

    const bool isInPositiveDeadZone = joystickMoved->position > Input::instance().thumbStickDeadZone;
    const bool isInNegativeDeadZone = joystickMoved->position < -Input::instance().thumbStickDeadZone;

    previousLeftThumbstickMoved = currentLeftThumbstickMoved;
    currentLeftThumbstickMoved = isInPositiveDeadZone || isInNegativeDeadZone;
    return previousLeftThumbstickMoved != currentLeftThumbstickMoved && currentLeftThumbstickMoved;
}


bool Dualsense::wasDpadMoved(const sf::Event::JoystickMoved* joystickMoved)
{
    if (!isSupported(sf::Joystick::getIdentification(joystickMoved->joystickId)))
        return false;

    const bool isInPositiveDeadZone = joystickMoved->position > Input::instance().thumbStickDeadZone;
    const bool isInNegativeDeadZone = joystickMoved->position < -Input::instance().thumbStickDeadZone;

    previousDpadMoved = currentDpadMoved;
    currentDpadMoved = isInPositiveDeadZone || isInNegativeDeadZone;
    return previousDpadMoved != currentDpadMoved && currentDpadMoved;
}


sf::Vector2f Dualsense::leftThumbStickPosition(const int index, const sf::Joystick::Identification& identification) const
{
    if (!isSupported(identification))
        return {0.f, 0.f};

    return Input::instance().thumbStickPosition(index, sf::Joystick::Axis::X, sf::Joystick::Axis::Y);
}


sf::Vector2f Dualsense::rightThumbStickPosition(const int index, const sf::Joystick::Identification& identification) const
{
    if (!isSupported(identification))
        return {0.f, 0.f};

    return Input::instance().thumbStickPosition(index, sf::Joystick::Axis::Z, sf::Joystick::Axis::R);
}


bool Dualsense::isBackButton(const sf::Event::JoystickButtonReleased* joystickButtonReleased) const
{
    if (!isSupported(sf::Joystick::getIdentification(joystickButtonReleased->joystickId)))
        return false;

    return joystickButtonReleased->button == 8;
}


bool Dualsense::isStartButton(const sf::Event::JoystickButtonReleased* joystickButtonReleased) const
{
    if (!isSupported(sf::Joystick::getIdentification(joystickButtonReleased->joystickId)))
        return false;

    return joystickButtonReleased->button == 9;
}


bool Dualsense::isXButton(const sf::Event::JoystickButtonReleased* joystickButtonReleased) const
{
    if (!isSupported(sf::Joystick::getIdentification(joystickButtonReleased->joystickId)))
        return false;

    return joystickButtonReleased->button == 1;
}

bool Dualsense::isSquareButton(const sf::Event::JoystickButtonReleased* joystickButtonReleased) const
{
    if (!isSupported(sf::Joystick::getIdentification(joystickButtonReleased->joystickId)))
        return false;

    return joystickButtonReleased->button == 0;
}


bool Dualsense::isCircleButton(const sf::Event::JoystickButtonReleased* joystickButtonReleased) const
{
    if (!isSupported(sf::Joystick::getIdentification(joystickButtonReleased->joystickId)))
        return false;

    return joystickButtonReleased->button == 2;
}


bool Dualsense::isTriangleButton(const sf::Event::JoystickButtonReleased* joystickButtonReleased) const
{
    if (!isSupported(sf::Joystick::getIdentification(joystickButtonReleased->joystickId)))
        return false;

    return joystickButtonReleased->button == 3;
}