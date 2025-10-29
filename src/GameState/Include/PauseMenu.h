#ifndef PAUSEMENU_H
#define PAUSEMENU_H
#include "IGameState.h"
#include "../../Content/Include/Art.h"
#include "SFML/Graphics/Text.hpp"


struct PauseMenu final : IGameState
{
    PauseMenu();

    static PauseMenu &instance()
    {
        static auto *instance = new PauseMenu;
        return *instance;
    }

    sf::Text pausedText {Art::instance().font, {"== [ paused ] =="}, 72};
    sf::Text highScoreHeaderText {Art::instance().font, {"== [ highscore ] =="}, 30};
    sf::Text highScoreText {Art::instance().font, {}, 30};

    void processKeyReleased(const sf::Event::KeyReleased* keyReleased) override;
    void processJoystickButtonReleased(const sf::Event::JoystickButtonReleased* joystickButtonReleased) override;
    void processJoystickAxisMoved(const sf::Event::JoystickMoved* joystickMoved) override;
    void update() override;
    void renderGaussianBlur() override;
    void renderToScreen() override;
};



#endif //PAUSEMENU_H
