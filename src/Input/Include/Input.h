#ifndef INPUT_H
#define INPUT_H
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/Joystick.hpp"


enum struct InputMode
{
    MouseAndKeyboard,
    Joystick
};

struct Input
{
    Input();

    static Input& instance()
    {
        static auto* instance = new Input;
        return *instance;
    }

    float thumbStickDeadZone = 15; // SFML dead zone values from -100 to 100
    float triggerDeadZone = 50; // SFML dead zone values  from -100 to 100
    bool hasValidXboxInput = false;
    bool hasValidDualsenseInput = false;
    InputMode inputMode = InputMode::MouseAndKeyboard;

    [[nodiscard]] bool isMouseVisible() const;
    [[nodiscard]] sf::Vector2f getMovementDirection() const;
    [[nodiscard]] sf::Vector2f getAimDirection(const sf::Vector2f& fromPosition) const;
    [[nodiscard]] bool isAxisRightThumbstick(const sf::Event::JoystickMoved* joystickMoved) const;
    [[nodiscard]] bool isAxisRightTrigger(const sf::Event::JoystickMoved* joystickMoved) const;
    [[nodiscard]] bool wasRightTriggerReleased(const sf::Event::JoystickMoved* joystickMoved);
    [[nodiscard]] sf::Vector2f thumbStickPosition(int index, sf::Joystick::Axis, sf::Joystick::Axis) const;
    [[nodiscard]] bool isBackButton(const sf::Event::JoystickButtonReleased* joystickButtonReleased);
    [[nodiscard]] bool isStartButton(const sf::Event::JoystickButtonReleased* joystickButtonReleased);
    [[nodiscard]] bool isPrimaryButton(const sf::Event::JoystickButtonReleased* joystickButtonReleased);
    [[nodiscard]] bool isSecondaryButton(const sf::Event::JoystickButtonReleased* joystickButtonReleased);
    [[nodiscard]] bool isTertiaryButton(const sf::Event::JoystickButtonReleased* joystickButtonReleased);
    [[nodiscard]] bool isQuaternaryButton(const sf::Event::JoystickButtonReleased* joystickButtonReleased);
    void checkConnectedDevice();
    void update();
    void draw() const;
};


#endif //INPUT_H
