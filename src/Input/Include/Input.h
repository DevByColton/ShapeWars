#ifndef INPUT_H
#define INPUT_H
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/Joystick.hpp"


enum struct AimMode
{
    Mouse,
    Keyboard,
    Joystick
};

class Input
{
private:
    AimMode aimMode = AimMode::Mouse;

public:
    Input();

    static Input& instance()
    {
        static auto* instance = new Input;
        return *instance;
    }

    float thumbStickDeadZone = 15; // SFML dead zone values from -100 to 100
    float triggerDeadZone = 50; // SFML dead zone values  from -100 to 100

    [[nodiscard]] bool isMouseVisible() const;
    [[nodiscard]] sf::Vector2f getMovementDirection() const;
    [[nodiscard]] sf::Vector2f getAimDirection(const sf::Vector2f& fromPosition) const;
    [[nodiscard]] bool isAxisRightThumbstick(const sf::Event::JoystickMoved* joystickMoved) const;
    [[nodiscard]] bool isAxisRightTrigger(const sf::Event::JoystickMoved* joystickMoved) const;
    [[nodiscard]] bool wasRightTriggerReleased(const sf::Event::JoystickMoved* joystickMoved);
    [[nodiscard]] sf::Vector2f thumbStickPosition(const int index, sf::Joystick::Axis, sf::Joystick::Axis) const;
    [[nodiscard]] bool isBackButton(const sf::Event::JoystickButtonReleased* joystickButtonReleased);
    [[nodiscard]] bool isStartButton(const sf::Event::JoystickButtonReleased* joystickButtonReleased);
    void mouseMoved();
    void setAimMode(AimMode aimMode);
    void checkConnectedDevice();
    void update();
    void draw() const;
};


#endif //INPUT_H
