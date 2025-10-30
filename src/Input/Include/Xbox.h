#ifndef XBOX_H
#define XBOX_H
#include <string>
#include "SFML/Window/Event.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                       Mappings for Xbox controllers
//
// sf::Joystick::Axis::X         Left stick x value, left is negative and right is positive
// sf::Joystick::Axis::Y         Left stick y value, up is negative and down is positive
// sf::Joystick::Axis::Z         Left AND Right Trigger: Left trigger is positive, right trigger is negative
// sf::Joystick::Axis::R               ???
// sf::Joystick::Axis::U         Right stick x value
// sf::Joystick::Axis::V         Right stick y value
// sf::Joystick::Axis::PovX      Left and right on the dpad, left is negative and right is positive
// sf::Joystick::Axis::PovY      Up and down on the dpad, up is positive and down is negative
//
//
// 0   A
// 1   B
// 2   X
// 3   Y
// 4   left bumper
// 5   right bumper
// 6   back/select
// 7   start
// 8   left thumbstick click
// 9   right thumbstick click
//
// 10 ... 31 buttons I don't care about
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Xbox {
private:
    static constexpr std::string XBOX {"xbox"};
    static constexpr std::string XINPUT {"xinput"};
    bool previousRightTriggerPressed = false;
    bool currentRightTriggerPressed = false;
    bool previousDpadMoved = false;
    bool currentDpadMoved = false;
    bool previousLeftThumbstickMoved = false;
    bool currentLeftThumbstickMoved = false;

public:
    static Xbox &instance()
    {
        static auto *instance = new Xbox;
        return *instance;
    }

    [[nodiscard]] bool isSupported(const sf::Joystick::Identification& identification) const;
    [[nodiscard]] bool isDpadX(const sf::Event::JoystickMoved* joystickMoved) const;
    [[nodiscard]] bool isDpadY(const sf::Event::JoystickMoved* joystickMoved) const;
    [[nodiscard]] bool isLeftThumbstickX(const sf::Event::JoystickMoved* joystickMoved) const;
    [[nodiscard]] bool isLeftThumbstickY(const sf::Event::JoystickMoved* joystickMoved) const;
    [[nodiscard]] bool isAxisRightThumbstick(const sf::Event::JoystickMoved* joystickMoved) const;
    [[nodiscard]] bool isAxisRightTrigger(const sf::Event::JoystickMoved* joystickMoved) const;
    [[nodiscard]] bool wasRightTriggerReleased(const sf::Event::JoystickMoved* joystickMoved);
    [[nodiscard]] bool wasDpadMoved(const sf::Event::JoystickMoved* joystickMoved);
    [[nodiscard]] bool wasLeftThumbstickMoved(const sf::Event::JoystickMoved* joystickMoved);
    [[nodiscard]] sf::Vector2f leftThumbStickPosition(int index, const sf::Joystick::Identification& identification) const;
    [[nodiscard]] sf::Vector2f rightThumbStickPosition(int index, const sf::Joystick::Identification& identification) const;
    [[nodiscard]] bool isBackButton(const sf::Event::JoystickButtonReleased* joystickButtonReleased) const;
    [[nodiscard]] bool isStartButton(const sf::Event::JoystickButtonReleased* joystickButtonReleased) const;
    [[nodiscard]] bool isAButton(const sf::Event::JoystickButtonReleased* joystickButtonReleased) const;
    [[nodiscard]] bool isBButton(const sf::Event::JoystickButtonReleased* joystickButtonReleased) const;
    [[nodiscard]] bool isXButton(const sf::Event::JoystickButtonReleased* joystickButtonReleased) const;
    [[nodiscard]] bool isYButton(const sf::Event::JoystickButtonReleased* joystickButtonReleased) const;
};



#endif //XBOX_H
