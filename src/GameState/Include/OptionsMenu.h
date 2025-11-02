#ifndef OPTIONSMENU_H
#define OPTIONSMENU_H
#include "IGameState.h"


struct OptionsMenu final : IGameState {

    static OptionsMenu &instance()
    {
        static auto *instance = new OptionsMenu;
        return *instance;
    }

    void update() override;
    void renderGaussianBlur() override;
    void renderToScreen() override;
    void processMouseReleased(const sf::Event::MouseButtonReleased* mouseReleased) override;
    void processKeyReleased(const sf::Event::KeyReleased* keyReleased) override;
    void processJoystickButtonReleased(const sf::Event::JoystickButtonReleased* joystickButtonReleased) override;
    void processJoystickAxisMoved(const sf::Event::JoystickMoved* joystickMoved) override;
};



#endif //OPTIONSMENU_H
