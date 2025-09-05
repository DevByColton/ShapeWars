#ifndef INPUT_H
#define INPUT_H
#include "../Content/Include/Art.h"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Joystick.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// sf::Joystick::Axis::X         Left stick x value
// sf::Joystick::Axis::Y         Left stick y value
// sf::Joystick::Axis::Z         Left AND Right Trigger: Left trigger is positive, right trigger is negative
// sf::Joystick::Axis::R               ???
// sf::Joystick::Axis::U         Right stick x value
// sf::Joystick::Axis::V         Right stick y value
// sf::Joystick::Axis::PovX      Left and right on the dpad
// sf::Joystick::Axis::PovY      Up and down on the dpad (NOTE: up is positive and down is negative)
//
//
//                                 BUTTONS (based off xbox controller)
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

class Input {
private:
    enum class AimMode {
        Mouse,
        Keyboard,
        Controller
    };

    AimMode aimMode = AimMode::Mouse;
    float thumbStickDeadZone = 15; // SFML thumbsticks value from -100 to 100

    [[nodiscard]] bool isMouseVisible() const;
    [[nodiscard]] sf::Vector2f getThumbstickPosition(sf::Joystick::Axis, sf::Joystick::Axis) const;
    void setAimingMode();

public:
    Input() = default;
    ~Input() = default;

    static Input &instance() {
        static auto *instance = new Input;
        return *instance;
    }

    sf::Sprite mouseCursor {Art::instance().pointer};
    sf::Vector2f previousMousePosition {0.0, 0.0};
    sf::Vector2f currentMousePosition {0.0, 0.0};

    [[nodiscard]] sf::Vector2f getMovementDirection() const;
    [[nodiscard]] sf::Vector2f getAimDirection();
    void updateMousePosition();
    void update();
    void draw() const;
};



#endif //INPUT_H
