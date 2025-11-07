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


enum struct ButtonsOverride
{
    Keyboard,
    Xbox,
    Dualsense
};


struct Input
{
    Input();

    static Input& instance()
    {
        static auto* instance = new Input;
        return *instance;
    }

    float thumbStickDeadZone = 15.f; // SFML dead zone values from -100 to 100
    float triggerDeadZone = 50.f; // SFML dead zone values  from -100 to 100

    bool hasValidXboxInput = false;
    bool hasValidDualsenseInput = false;
    InputMode inputMode = InputMode::MouseAndKeyboard;

    bool isButtonsOverrideActive = false;
    ButtonsOverride buttonsOverride = ButtonsOverride::Xbox;

    [[nodiscard]] bool isMouseVisible() const;
    [[nodiscard]] static sf::Vector2f getMovementDirection(bool withWasdMovement);
    [[nodiscard]] sf::Vector2f getAimDirection(const sf::Vector2f& fromPosition) const;
    [[nodiscard]] sf::Vector2f thumbStickPosition(int index, sf::Joystick::Axis, sf::Joystick::Axis) const;
    [[nodiscard]] static bool isDpadX(const sf::Event::JoystickMoved* joystickMoved);
    [[nodiscard]] static bool isDpadY(const sf::Event::JoystickMoved* joystickMoved);
    [[nodiscard]] static bool isLeftThumbstickX(const sf::Event::JoystickMoved* joystickMoved);
    [[nodiscard]] static bool isLeftThumbstickY(const sf::Event::JoystickMoved* joystickMoved);
    [[nodiscard]] static bool isAxisRightThumbstick(const sf::Event::JoystickMoved* joystickMoved);
    [[nodiscard]] static bool isAxisRightTrigger(const sf::Event::JoystickMoved* joystickMoved);
    [[nodiscard]] static bool wasRightTriggerReleased(const sf::Event::JoystickMoved* joystickMoved);
    [[nodiscard]] static bool wasDpadMoved(const sf::Event::JoystickMoved* joystickMoved);
    [[nodiscard]] static bool wasLeftThumbstickMoved(const sf::Event::JoystickMoved* joystickMoved);
    [[nodiscard]] static bool isBackButton(const sf::Event::JoystickButtonReleased* joystickButtonReleased);
    [[nodiscard]] static bool isStartButton(const sf::Event::JoystickButtonReleased* joystickButtonReleased);
    [[nodiscard]] static bool isPrimaryButton(const sf::Event::JoystickButtonReleased* joystickButtonReleased);
    [[nodiscard]] static bool isSecondaryButton(const sf::Event::JoystickButtonReleased* joystickButtonReleased);
    [[nodiscard]] static bool isTertiaryButton(const sf::Event::JoystickButtonReleased* joystickButtonReleased);
    [[nodiscard]] static bool isQuaternaryButton(const sf::Event::JoystickButtonReleased* joystickButtonReleased);
    void checkConnectedDevice();
    void update();
    void draw() const;
};


#endif //INPUT_H
