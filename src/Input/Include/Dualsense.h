#ifndef DUALSENSE_H
#define DUALSENSE_H
#include <string>
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/Joystick.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                       Mappings for Dualsense controllers
//
// sf::Joystick::Axis::X         Left stick x value, left is negative and right is positive
// sf::Joystick::Axis::Y         Left stick y value, up is negative and down is positive
// sf::Joystick::Axis::Z         Right stick x value
// sf::Joystick::Axis::R         Right stick y value
// sf::Joystick::Axis::U         Left trigger, starts at -100 and fully pushed is 100
// sf::Joystick::Axis::V         Right trigger, starts at -100 and fully pushed is 100
// sf::Joystick::Axis::PovX      Left and right on the dpad, left is negative and right is positive
// sf::Joystick::Axis::PovY      Up and down on the dpad, up is positive and down is negative
//
//
// 0   square
// 1   x
// 2   circle
// 3   triangle
// 4   left bumper
// 5   right bumper
// ...
// 8   back
// 9   start
// 10  left stick click
// 11  right stick click
// 12  home/ps logo
// 13  big button
//
// 14 ... 31 buttons I don't care about
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class Dualsense {
private:
    static constexpr std::string PS5 {"ps5"};
    static constexpr std::string PS4 {"ps4"};
    static constexpr std::string DUALSENSE {"dualsense"};
    static constexpr std::string PLAYSTATION {"playstation"};
    bool previousRightTriggerPressed = false;
    bool currentRightTriggerPressed = false;
    bool previousDpadMoved = false;
    bool currentDpadMoved = false;
    bool previousLeftThumbstickMoved = false;
    bool currentLeftThumbstickMoved = false;

public:
    static Dualsense &instance()
    {
        static auto *instance = new Dualsense;
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
    [[nodiscard]] bool wasLeftThumbstickMoved(const sf::Event::JoystickMoved* joystickMoved);
    [[nodiscard]] bool wasDpadMoved(const sf::Event::JoystickMoved* joystickMoved);
    [[nodiscard]] float dpadXPosition(int index, const sf::Joystick::Identification& identification) const;
    [[nodiscard]] float dpadYPosition(int index, const sf::Joystick::Identification& identification) const;
    [[nodiscard]] sf::Vector2f leftThumbStickPosition(int index, const sf::Joystick::Identification& identification) const;
    [[nodiscard]] sf::Vector2f rightThumbStickPosition(int index, const sf::Joystick::Identification& identification) const;
    [[nodiscard]] bool isBackButton(const sf::Event::JoystickButtonReleased* joystickButtonReleased) const;
    [[nodiscard]] bool isStartButton(const sf::Event::JoystickButtonReleased* joystickButtonReleased) const;
    [[nodiscard]] bool isXButton(const sf::Event::JoystickButtonReleased* joystickButtonReleased) const;
    [[nodiscard]] bool isSquareButton(const sf::Event::JoystickButtonReleased* joystickButtonReleased) const;
    [[nodiscard]] bool isCircleButton(const sf::Event::JoystickButtonReleased* joystickButtonReleased) const;
    [[nodiscard]] bool isTriangleButton(const sf::Event::JoystickButtonReleased* joystickButtonReleased) const;
};



#endif //DUALSENSE_H
