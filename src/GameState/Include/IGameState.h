#ifndef IGAMESTATE_H
#define IGAMESTATE_H
#include "SFML/Window/Event.hpp"


struct IGameState
{
    virtual ~IGameState() = default;

    virtual void update() = 0;
    virtual void renderGaussianBlur() = 0;
    virtual void renderToScreen() = 0;
    virtual void processMouseReleased(const sf::Event::MouseButtonReleased* mouseReleased) = 0;
    virtual void processKeyReleased(const sf::Event::KeyReleased* keyReleased) = 0;
    virtual void processJoystickButtonReleased(const sf::Event::JoystickButtonReleased* joystickButtonReleased) = 0;
    virtual void processJoystickAxisMoved(const sf::Event::JoystickMoved* joystickMoved) = 0;
};



#endif //IGAMESTATE_H
