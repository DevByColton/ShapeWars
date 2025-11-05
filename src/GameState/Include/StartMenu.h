#ifndef STARTMENU_H
#define STARTMENU_H
#include <functional>
#include <random>
#include "ActiveMenuOptionIndicator.h"
#include "IGameState.h"
#include "../../GameRoot.h"
#include "../../Content/Include/Art.h"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Graphics/RenderTexture.hpp"
#include "SFML/Graphics/Sprite.hpp"


struct StartMenu final : IGameState
{
    StartMenu();

    static StartMenu &instance()
    {
        static auto *instance = new StartMenu;
        return *instance;
    }

    struct MenuOption final : sf::Text
    {
        explicit MenuOption(const sf::Font& font, const sf::String& string, const unsigned int characterSize)
            : sf::Text(font, string, characterSize)
        {
            // Mute the menu option by default
            setFillColor(MUTED_TEXT_COLOR);
            setStyle(Bold);
        }

        static constexpr sf::Color MUTED_TEXT_COLOR {185, 185, 185, 255};
        std::function<void()> onSelect {};
    };

    // Menus options area
    static constexpr int MENU_OPTIONS_COUNT = 3;
    int activeMenuOptionIndex = 0;
    sf::RenderTexture menuOptionsTexture = {{600, 250}};
    sf::Sprite menuOptionsSprite {menuOptionsTexture.getTexture()};
    MenuOption start {Art::instance().majorMonoFont, {"start"}, 60};
    MenuOption options {Art::instance().majorMonoFont, {"options"}, 60};
    MenuOption quit {Art::instance().majorMonoFont, {"quit"}, 60};
    MenuOption* activeMenuOption = &start;
    std::array<MenuOption*, MENU_OPTIONS_COUNT> menuOptionPtrs {&start, &options, &quit};
    OptionIndicator optionIndicator {};

    // Menu and title options transitions
    static constexpr float TRANSITION_DURATION = 0.4f;
    bool isTransitioningIn = false;
    bool isTransitioningOut = false;
    float transitionTime = 0.f;
    const sf::Vector2f menuOptionsOffScreenPosition {-330.f, GameRoot::instance().windowSizeF.y / 2.f};
    const sf::Vector2f menuOptionsOnScreenPosition {325.f, GameRoot::instance().windowSizeF.y / 2.f};
    const sf::Vector2f titleOffScreenPosition {
        GameRoot::instance().windowSizeF.x + 355.f,
        GameRoot::instance().windowSizeF.y / 2.f
    };
    const sf::Vector2f titleOnScreenPosition {
        GameRoot::instance().windowSizeF.x - 350.f,
        GameRoot::instance().windowSizeF.y / 2.f
    };

    // Title area
    sf::Text shapeText {Art::instance().majorMonoFont, {"SHAPE"}, 140};
    sf::Text warsText {Art::instance().majorMonoFont, {"WARS"}, 140};
    sf::RenderTexture titleTexture = {{600, 250}};
    sf::Sprite title {titleTexture.getTexture()};

    // Background particles
    static constexpr float TIME_UNTIL_NEW_EXPLOSION_DURATION = 0.08f;
    float timeUntilNewExplosionElapsed = TIME_UNTIL_NEW_EXPLOSION_DURATION;
    std::default_random_engine particleDurationRandEngine {std::random_device{}()};
    std::default_random_engine widthRandEngine {std::random_device{}()};
    std::default_random_engine heightRandEngine {std::random_device{}()};
    std::uniform_real_distribution<float> particleDurationDistribution {0.8f, 1.2f};
    std::uniform_real_distribution<float> widthDistribution {20.f, GameRoot::instance().windowSizeF.x - 20.f};
    std::uniform_real_distribution<float> heightDistribution {20.f, GameRoot::instance().windowSizeF.y - 20.f};

    void processMouseMoved(const sf::Event::MouseMoved* mouseMoved) override;
    void processMouseReleased(const sf::Event::MouseButtonReleased* mouseReleased) override;
    void processMousePressed(const sf::Event::MouseButtonPressed* mousePressed) override;
    void processMouseWheelScrolledEvent(const sf::Event::MouseWheelScrolled* mouseWheelScrolled) override;
    void processKeyPressed(const sf::Event::KeyPressed* keyPressed) override;
    void processKeyReleased(const sf::Event::KeyReleased* keyReleased) override;
    void processJoystickButtonReleased(const sf::Event::JoystickButtonReleased* joystickButtonReleased) override;
    void processJoystickAxisMoved(const sf::Event::JoystickMoved* joystickMoved) override;
    void update() override;
    void renderGaussianBlur() override;
    void renderToScreen() override;
    void setActiveMenuOption(MenuOption* nextMenuOption);
    void transitionMenuAndTitleIn();
    bool transitionMenuAndTitleOut();
    void moveToNextMenuOption(float direction);
    void updateBackground();
};



#endif //STARTMENU_H
