#ifndef STARTMENU_H
#define STARTMENU_H
#include <functional>
#include <random>
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
            : sf::Text(font, string, characterSize){}

        std::function<void()> onSelect {};
    };

    struct ActiveMenuOptionIndicator final : sf::Sprite
    {
        explicit ActiveMenuOptionIndicator(const sf::Texture& texture) : Sprite(texture)
        {
            setOrigin(getLocalBounds().getCenter());
            setScale({0.4f, 0.4f});
        }

        sf::Vector2f previousPosition {0.f, 0.f};
        sf::Vector2f targetPosition {0.f, 0.f};

        void setActive(const sf::Vector2f& targetPosition);
        void transition(float time);
    };

    // Menus options area
    static constexpr int MENU_OPTIONS_COUNT = 3;
    static constexpr float TRANSITION_DURATION = 0.075f;
    float transitionTime = 0.f;
    bool isActiveOptionIndicatorTransitioning = false;
    int activeMenuOptionIndex = 0;
    sf::RenderTexture menuOptionsTexture = {{600, 250}};
    sf::Sprite menuOptionsSprite {menuOptionsTexture.getTexture()};
    MenuOption start {Art::instance().majorMonoFont, {"start"}, 60};
    MenuOption options {Art::instance().majorMonoFont, {"options"}, 60};
    MenuOption quit {Art::instance().majorMonoFont, {"quit"}, 60};
    MenuOption* activeMenuOption = &start;
    std::array<MenuOption*, MENU_OPTIONS_COUNT> menuOptionPtrs {&start, &options, &quit};
    ActiveMenuOptionIndicator leftIndicator {Art::instance().shapeKeeperCore};
    ActiveMenuOptionIndicator rightIndicator {Art::instance().shapeKeeperCore};

    // Shape wars area
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

    void processKeyReleased(const sf::Event::KeyReleased* keyReleased) override;
    void processJoystickButtonReleased(const sf::Event::JoystickButtonReleased* joystickButtonReleased) override;
    void processJoystickAxisMoved(const sf::Event::JoystickMoved* joystickMoved) override;
    void update() override;
    void renderGaussianBlur() override;
    void renderToScreen() override;
    void moveToNextMenuOption(float direction);
    void updateBackground();
    void updateMenuOptions();
};



#endif //STARTMENU_H
