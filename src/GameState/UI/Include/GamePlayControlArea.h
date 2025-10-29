#ifndef GAMEPLAYCONTROLAREA_H
#define GAMEPLAYCONTROLAREA_H
#include "../../../GameRoot.h"
#include "../../../Input/Include/Buttons.h"
#include "SFML/Graphics/RenderTexture.hpp"


struct GamePlayControlArea {
    GamePlayControlArea();

    struct ResourceCounter final : sf::Sprite
    {
        explicit ResourceCounter(const sf::Texture &texture);

        sf::Sprite frame {Art::instance().livesAndNukesFrame};
    };

    static constexpr float TRANSITION_DURATION = 0.4f;
    const sf::Vector2f controlAreaOffScreenPosition {
        GameRoot::instance().windowSizeF.x / 2.f,
        GameRoot::instance().windowSizeF.y + 75.f
    };
    const sf::Vector2f controlAreaOnScreenPosition {
        GameRoot::instance().windowSizeF.x / 2.f,
        GameRoot::instance().windowSizeF.y - 40.f
    };
    bool isTransitioningIn = false;
    bool isTransitioningOut = false;
    float transitionTime = 0.f;
    sf::RenderTexture controlAreaTexture {{700, 150}};
    sf::Sprite controlArea {controlAreaTexture.getTexture()};
    Buttons buttons {};
    ResourceCounter lives {Art::instance().player};
    ResourceCounter nukes {Art::instance().nuke};

    void transitionIn();
    void transitionOut();
    void drawToBlur();
    void drawToScreen();
    void drawLivesAndNukes();
};



#endif //GAMEPLAYCONTROLAREA_H
