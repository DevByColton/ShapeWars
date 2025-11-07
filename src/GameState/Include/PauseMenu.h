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

    sf::Text pausedText {Art::instance().turretRoadFont, {"== [ paused ] =="}, 72};
    sf::Text highScoreHeaderText {Art::instance().turretRoadFont, {"== [ highscore ] =="}, 30};
    sf::Text highScoreText {Art::instance().turretRoadFont, {}, 30};

    void processMouseMoved(const sf::Event::MouseMoved* mouseMoved) override;
    void processMouseReleased(const sf::Event::MouseButtonReleased* mouseReleased) override;
    void processMouseWheelScrolledEvent(const sf::Event::MouseWheelScrolled* mouseWheelScrolled) override;
    void processKeyPressed(const sf::Event::KeyPressed* keyPressed) override;
    void processKeyReleased(const sf::Event::KeyReleased* keyReleased) override;
    void processMousePressed(const sf::Event::MouseButtonPressed* mousePressed) override;
    void processJoystickButtonReleased(const sf::Event::JoystickButtonReleased* joystickButtonReleased) override;
    void processJoystickAxisMoved(const sf::Event::JoystickMoved* joystickMoved) override;
    void update() override;
    void renderGaussianBlur() override;
    void renderToScreen() override;
    void resume();
};



#endif //PAUSEMENU_H
