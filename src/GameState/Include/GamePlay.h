#ifndef GAMEPLAY_H
#define GAMEPLAY_H
#include "IGameState.h"
#include "../UI/Include/GamePlayHUD.h"


struct GamePlay final : IGameState
{
    static GamePlay &instance()
    {
        static auto *instance = new GamePlay;
        return *instance;
    }

    GamePlayHUD gamePlayHud {};

    void processKeyReleased(const sf::Event::KeyReleased* keyReleased) override;
    void processJoystickButtonReleased(const sf::Event::JoystickButtonReleased* joystickButtonReleased) override;
    void processJoystickAxisMoved(const sf::Event::JoystickMoved* joystickMoved) override;
    void update() override;
    void renderGaussianBlur() override;
    void renderToScreen() override;
};



#endif //GAMEPLAY_H
