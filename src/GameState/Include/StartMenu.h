#ifndef STARTMENU_H
#define STARTMENU_H
#include "IGameState.h"


struct StartMenu final : IGameState
{
    static StartMenu &instance()
    {
        static auto *instance = new StartMenu;
        return *instance;
    }

    void processKeyReleased(const sf::Event::KeyReleased* keyReleased) override;
    void processJoystickButtonReleased(const sf::Event::JoystickButtonReleased* joystickButtonReleased) override;
    void processJoystickAxisMoved(const sf::Event::JoystickMoved* joystickMoved) override;
    void update() override;
    void renderGaussianBlur() override;
    void renderToScreen() override;
};



#endif //STARTMENU_H
