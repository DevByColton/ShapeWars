#ifndef OPTIONSMENU_H
#define OPTIONSMENU_H
#include <functional>
#include "ActiveMenuOptionIndicator.h"
#include "IGameState.h"
#include "../../GameRoot.h"
#include "../../Content/Include/Art.h"
#include "SFML/Graphics/RenderTexture.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Text.hpp"


struct OptionsMenu final : IGameState {
    OptionsMenu();

    static OptionsMenu &instance()
    {
        static auto *instance = new OptionsMenu;
        return *instance;
    }

    struct IOption
    {
        virtual ~IOption() = default;

        static constexpr sf::Color MUTED_TEXT_COLOR {185, 185, 185, 255};
        sf::RenderTexture containerTexture {{1400, 50}};
        sf::Sprite containerSprite {containerTexture.getTexture()};
        sf::Text label {Art::instance().majorMonoFont, {""}, 60};
    };

    struct SwitchOption final : IOption
    {
        SwitchOption(const std::string& label, bool checked, const sf::Vector2f& position);

        bool checked = true;
        sf::Sprite switchChecked {Art::instance().switchChecked};
        sf::Sprite switchUnchecked {Art::instance().switchUnchecked};
        std::function<void()> onToggle {};

        void draw(sf::RenderTexture& renderTexture);
    };

    struct SliderOption final : IOption
    {
        SliderOption(const std::string& label, const sf::Vector2f& position);

        static constexpr float MAX = 100.f;
        static constexpr float MIN = 0.f;
        float value = 75.f;
        sf::Sprite knob {Art::instance().sliderOptionKnob};
        sf::Sprite fill {Art::instance().sliderOptionBarFill};
        sf::Sprite outline {Art::instance().sliderOptionBarOutline};
        std::function<void()> onUpdate {};
        std::function<void()> onPress {};
        std::function<void()> onRelease {};

        void updateValue(float next);
        void setValuePositions();
        void draw(sf::RenderTexture& renderTexture);
    };

    struct ActiveSliderOption
    {
        float originalValue = 0.f;
        SliderOption* sliderOption {nullptr};
        sf::Vector2f mouseStartingPosition {};
    };

    // Options background and title
    const sf::Color backgroundColor {35,31,32, 225};
    sf::RenderTexture optionsBackgroundTexture = {{GameRoot::instance().renderWindow.getSize().x, GameRoot::instance().renderWindow.getSize().y}};
    sf::Sprite optionBackground {optionsBackgroundTexture.getTexture()};
    sf::Text title {Art::instance().majorMonoFont, {"OPTIONS"}, 100};

    // Options
    static constexpr int OPTIONS_COUNT = 4;
    int activeOptionIndex = 0;
    SwitchOption vsync {"vsync", true, {GameRoot::instance().renderWindow.getSize().x / 2.f, 250.f}};
    SliderOption musicVol {"music vol", {vsync.containerSprite.getPosition().x, vsync.containerSprite.getPosition().y + 75.f}};
    SliderOption sfxVol {"sfx vol", {musicVol.containerSprite.getPosition().x, musicVol.containerSprite.getPosition().y + 75.f}};
    SwitchOption buttonsOverride {"buttons override", false, {sfxVol.containerSprite.getPosition().x, sfxVol.containerSprite.getPosition().y + 75.f}};
    OptionIndicator optionIndicator {};
    IOption* activeOption {&vsync};
    ActiveSliderOption activeSliderOption {};
    std::array<IOption*, OPTIONS_COUNT> optionPtrs {&vsync, &musicVol, &sfxVol, &buttonsOverride};

    IGameState* openedFrom {nullptr};

    void open(IGameState* openedFrom);
    void close();
    void setActiveOption(IOption* option);
    void toggleVsync();
    void startSliding(SliderOption* sliderOption);
    void endSliding();
    void toggleButtonOverride();
    void moveToNextMenuOption(float direction);
    void update() override;
    void renderGaussianBlur() override;
    void renderToScreen() override;
    void processMouseMoved(const sf::Event::MouseMoved* mouseMoved) override;
    void processMouseReleased(const sf::Event::MouseButtonReleased* mouseReleased) override;
    void processMousePressed(const sf::Event::MouseButtonPressed* mousePressed) override;
    void processMouseWheelScrolledEvent(const sf::Event::MouseWheelScrolled* mouseWheelScrolled) override;
    void processKeyPressed(const sf::Event::KeyPressed* keyPressed) override;
    void processKeyReleased(const sf::Event::KeyReleased* keyReleased) override;
    void processJoystickButtonReleased(const sf::Event::JoystickButtonReleased* joystickButtonReleased) override;
    void processJoystickAxisMoved(const sf::Event::JoystickMoved* joystickMoved) override;
};



#endif //OPTIONSMENU_H
