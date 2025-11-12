#ifndef HIGHSCOREAREA_H
#define HIGHSCOREAREA_H
#include "../../Content/Include/Art.h"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Graphics/RenderTexture.hpp"
#include "SFML/Graphics/Sprite.hpp"


struct HighscoreArea {
    HighscoreArea(bool withTransition, const sf::Vector2f& onScreenPosition, const sf::Vector2f& offScreenPosition);

    static constexpr float TRANSITION_DURATION = 0.4f;
    float transitionTime = 0.f;
    bool isTransitioningIn = false;
    bool isTransitioningOut = false;
    bool withTransition = false;
    sf::Vector2f offScreenPosition {0.f, 0.f};
    sf::Vector2f onScreenPosition {0.f, 0.f};

    sf::RenderTexture containerTexture = {{500, 125}};
    sf::Sprite containerSprite {containerTexture.getTexture()};
    sf::Text label {Art::instance().turretRoadFont, {"==[ highscore ]=="}, 48};
    sf::Text highScoreText {Art::instance().turretRoadFont, {}, 42};

    void startOnScreen();
    void startOffScreen();
    void transitionIn();
    void transitionOut();
    void update();
    void alignHighscoreText();
    void drawToScreen();
    void drawToTexture(sf::RenderTexture& texture);
};



#endif //HIGHSCOREAREA_H
