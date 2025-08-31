#include "Input.h"

#include "../GameRoot.h"
#include "../Entities/Include/PlayerShip.h"
#include "../Logger/Logger.h"
#include "../System/Include/Extensions.h"
#include "SFML/Window/Joystick.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "SFML/Window/Mouse.hpp"


bool Input::isMouseVisible() const {
    return aimMode == AimMode::Mouse;
}


sf::Vector2f Input::getThumbstickPosition(const sf::Joystick::Axis thumbstickX, const sf::Joystick::Axis thumbstickY) const {

    sf::Vector2f position {0.0, 0.0};

    // Get the positions of the left thumbstick, x and y
    const float x = sf::Joystick::getAxisPosition(0, thumbstickX);
    const float y = sf::Joystick::getAxisPosition(0, thumbstickY);

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


sf::Vector2f Input::getMovementDirection() const {

    // Get the left thumbstick position
    sf::Vector2f direction = getThumbstickPosition(sf::Joystick::Axis::X, sf::Joystick::Axis::Y);

    // Handle WASD input
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A))
        direction.x -= 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D))
        direction.x += 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W))
        direction.y -= 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S))
        direction.y += 1;

    // Normalize direction
    if (direction.lengthSquared() > 1)
        direction = direction.normalized();

    return direction;

}

sf::Vector2f Input::getAimDirection() {

    setAimingMode();

    sf::Vector2f direction = {0.0, 0.0};

    switch (aimMode) {
        case AimMode::Keyboard:
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left))
                direction.x -= 1;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right))
                direction.x += 1;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up))
                direction.y -= 1;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down))
                direction.y += 1;
            break;
        case AimMode::Controller:
            direction = getThumbstickPosition(sf::Joystick::Axis::U, sf::Joystick::Axis::V);
            break;
        case AimMode::Mouse:
        default:
            // Get the x and y coordinates of the mouse relative to the game window
            const float mousePositionX = static_cast<float>(sf::Mouse::getPosition(GameRoot::instance().renderWindow).x);
            const float mousePositionY = static_cast<float>(sf::Mouse::getPosition(GameRoot::instance().renderWindow).y);

            // Set the direction to point from the player to the mouse
            direction = {mousePositionX - PlayerShip::instance().getPosition().x, mousePositionY - PlayerShip::instance().getPosition().y};
    }

    // Normalize direction
    if (direction.lengthSquared() > 1)
        direction = direction.normalized();

    return direction;

}


void Input::setAimingMode() {

    // Check the current aiming mode
    const sf::Vector2f thumbstickPosition = getThumbstickPosition(sf::Joystick::Axis::U, sf::Joystick::Axis::V);
    const bool usingThumbstickToAim = thumbstickPosition.x != 0 || thumbstickPosition.y != 0;

    // Check aiming keys
    const bool areAimingKeysPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up) ||
                                      sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left) ||
                                      sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right) ||
                                      sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down);

    // Set the aim mode
    if (usingThumbstickToAim)
        aimMode = AimMode::Controller;
    else if (areAimingKeysPressed)
        aimMode = AimMode::Keyboard;
    else if (currentMousePosition != previousMousePosition)
        aimMode = AimMode::Mouse;

}

void Input::updateMousePosition() {

    // Set the previous mouse position
    previousMousePosition = currentMousePosition;

    // Get the x and y coordinates of the mouse relative to the game window
    const float mousePositionX = static_cast<float>(sf::Mouse::getPosition(GameRoot::instance().renderWindow).x);
    const float mousePositionY = static_cast<float>(sf::Mouse::getPosition(GameRoot::instance().renderWindow).y);

    currentMousePosition = {mousePositionX, mousePositionY};
    mouseCursor.setPosition(previousMousePosition);

}


void Input::draw() const {
    if (isMouseVisible())
        GameRoot::instance().renderWindow.draw(mouseCursor);
}


