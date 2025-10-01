#include "../Include/MouseAndKeyboard.h"
#include "../../GameRoot.h"


sf::Vector2f MouseAndKeyboard::movementDirection()
{
    sf::Vector2f direction {0.f, 0.f};

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A))
        direction.x -= 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D))
        direction.x += 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W))
        direction.y -= 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S))
        direction.y += 1;

    return direction;
}


sf::Vector2f MouseAndKeyboard::keysAimDirection()
{
    sf::Vector2f direction {0.f, 0.f};

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left))
        direction.x -= 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right))
        direction.x += 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up))
        direction.y -= 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down))
        direction.y += 1;

    return direction;
}


sf::Vector2f MouseAndKeyboard::mouseAimDirection(const sf::Vector2f& fromPosition) const
{
    return {mouseCursor.getPosition().x - fromPosition.x, mouseCursor.getPosition().y - fromPosition.y};
}


void MouseAndKeyboard::update()
{
    // Always update the mouse position to match actual mouse position relative to the game window
    const float mousePositionX = static_cast<float>(sf::Mouse::getPosition(GameRoot::instance().renderWindow).x);
    const float mousePositionY = static_cast<float>(sf::Mouse::getPosition(GameRoot::instance().renderWindow).y);
    mouseCursor.setPosition({mousePositionX, mousePositionY});
}


void MouseAndKeyboard::draw() const
{
    GameRoot::instance().renderWindow.draw(mouseCursor);
}
