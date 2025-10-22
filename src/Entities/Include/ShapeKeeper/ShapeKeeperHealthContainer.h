#ifndef SHAPEKEEPERHEALTHCONTAINER_H
#define SHAPEKEEPERHEALTHCONTAINER_H
#include "ShapeKeeperHealthBar.h"
#include "../../../GameRoot.h"
#include "../../../Content/Include/Art.h"
#include "SFML/Graphics/RenderTexture.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Text.hpp"


struct ShapeKeeperHealthContainer {
    ShapeKeeperHealthContainer();

    static constexpr float TRANSITION_DURATION = 0.4f;
    const sf::Vector2f healthWrapperOffScreenPosition {GameRoot::instance().windowSizeF.x / 2.f, -75.f};
    const sf::Vector2f healthWrapperOnScreenPosition {GameRoot::instance().windowSizeF.x / 2.f, 50.f};
    bool isTransitioningIn = false;
    bool isTransitioningOut = false;
    float transitionTime = 0.f;

    sf::Text headerText {Art::instance().shapeKeeperFont, {"SHAPE KEEPER"}, 60};
    sf::Sprite frame {Art::instance().shapeKeeperHealthFrame};
    sf::RenderTexture renderTexture {{frame.getTexture().getSize().x, frame.getTexture().getSize().y}};
    sf::Sprite sprite {renderTexture.getTexture()};

    ShapeKeeperHealthBar core {
        Art::instance().shapeKeeperHealthBarCore,
        {frame.getTexture().getSize().x - 65.f, 60.f}
    };

    ShapeKeeperHealthBar top {
        Art::instance().shapeKeeperHealthBarTop,
        {core.getPosition().x - 450.f, core.getPosition().y}
    };

    ShapeKeeperHealthBar middleLeft {
        Art::instance().shapeKeeperHealthBarMiddleLeft,
        {core.getPosition().x - 360.f, core.getPosition().y}
    };

    ShapeKeeperHealthBar middleRight {
        Art::instance().shapeKeeperHealthBarMiddleRight,
        {core.getPosition().x - 270.f, core.getPosition().y}
    };

    ShapeKeeperHealthBar bottomLeft {
        Art::instance().shapeKeeperHealthBarBottomLeft,
        {core.getPosition().x - 180, core.getPosition().y}
    };

    ShapeKeeperHealthBar bottomRight {
        Art::instance().shapeKeeperHealthBarBottomRight,
        {core.getPosition().x - 90.f, core.getPosition().y}
    };

    void transitionIn();
    void transitionOut();
    void draw();
    void drawText();
};



#endif //SHAPEKEEPERHEALTHCONTAINER_H
