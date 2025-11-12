#ifndef PAUSEMENU_H
#define PAUSEMENU_H
#include "ActiveMenuOptionIndicator.h"
#include "Background.h"
#include "HighscoreArea.h"
#include "IGameState.h"
#include "MenuOption.h"
#include "../../GameRoot.h"
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

    // Options background and title
    Background background {};
    sf::RenderTexture backgroundTexture = {{GameRoot::instance().renderWindow.getSize().x, GameRoot::instance().renderWindow.getSize().y}};
    sf::Sprite backgroundSprite {backgroundTexture.getTexture()};
    sf::Text paused {Art::instance().majorMonoFont, {"PAUSED"}, 100};

    static constexpr int MENU_OPTIONS_COUNT = 5;
    int activeMenuOptionIndex = 0;
    OptionIndicator optionIndicator {};
    MenuOption resume {Art::instance().turretRoadFont, {"resume"}, 48};
    MenuOption restart {Art::instance().turretRoadFont, {"restart"}, 48};
    MenuOption options {Art::instance().turretRoadFont, {"options"}, 48};
    MenuOption quitToMenu {Art::instance().turretRoadFont, {"quit to menu"}, 48};
    MenuOption quitToDesktop {Art::instance().turretRoadFont, {"quit to desktop"}, 48};
    MenuOption* activeMenuOption = &resume;
    std::array<MenuOption*, MENU_OPTIONS_COUNT> menuOptionPtrs {&resume, &restart, &options, &quitToMenu, &quitToDesktop};

    HighscoreArea highscoreArea {
        false,
        {GameRoot::instance().windowSizeF.x / 2.f, 100.f},
        {GameRoot::instance().windowSizeF.x / 2.f, 100.f}
    };

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
    void close();
    void resumeGameplay();
    void setActiveMenuOption(MenuOption* nextMenuOption, bool withPositionTransition);
    void moveToNextMenuOption(float direction);
};



#endif //PAUSEMENU_H
