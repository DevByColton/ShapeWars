#ifndef SHAPEKEEPER_H
#define SHAPEKEEPER_H
#include "../../GameRoot.h"
#include "../../Content/Include/Art.h"
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/RenderTexture.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Text.hpp"


class ShapeKeeper {
private:
    struct ShapeKeeperCore final : sf::Sprite
    {
        explicit ShapeKeeperCore(const sf::Texture& texture);

        void update();
    };

    struct ShapeKeeperBodyPart final : sf::Sprite
    {
        explicit ShapeKeeperBodyPart(const sf::Texture& texture, float coreXOffset, float coreYOffset);

        float coreXOffset = 0.f;
        float coreYOffset = 0.f;
        ShapeKeeperCore* core {nullptr};

        void update();
    };

    struct ShapeKeeperHealthBar final: sf::Sprite
    {
        explicit ShapeKeeperHealthBar(const sf::Texture& texture, const sf::Vector2f &position);

        sf::Sprite background;

        void update();
    };

    sf::Text shapeKeeperText {Art::instance().shapeKeeperFont, {"SHAPE KEEPER"}, 60};
    sf::Sprite shapeKeeperHealthFrame {Art::instance().shapeKeeperHealthFrame};
    sf::RenderTexture healthWrapper {{shapeKeeperHealthFrame.getTexture().getSize().x, shapeKeeperHealthFrame.getTexture().getSize().y}};
    sf::Sprite healthWrapperSprite {healthWrapper.getTexture()};

    ShapeKeeperCore core {Art::instance().shapeKeeperCore};
    ShapeKeeperHealthBar coreHealthBar {
        Art::instance().shapeKeeperHealthBarCore,
        {shapeKeeperHealthFrame.getTexture().getSize().x - 65.f, 60.f}
    };

    ShapeKeeperBodyPart top {Art::instance().shapeKeeperTop, 3.f, -167.f};
    ShapeKeeperHealthBar topHealthBar {
        Art::instance().shapeKeeperHealthBarTop,
        {coreHealthBar.getPosition().x - 450.f, coreHealthBar.getPosition().y}
    };


    ShapeKeeperBodyPart middleLeft {Art::instance().shapeKeeperMiddleLeft, -156.f, -49.f};
    ShapeKeeperHealthBar middleLeftHealthBar {
        Art::instance().shapeKeeperHealthBarMiddleLeft,
        {coreHealthBar.getPosition().x - 360.f, coreHealthBar.getPosition().y}
    };

    ShapeKeeperBodyPart middleRight {Art::instance().shapeKeeperMiddleRight, 165.f, -49.f};
    ShapeKeeperHealthBar middleRightHealthBar {
        Art::instance().shapeKeeperHealthBarMiddleRight,
        {coreHealthBar.getPosition().x - 270.f, coreHealthBar.getPosition().y}
    };

    ShapeKeeperBodyPart bottomLeft {Art::instance().shapeKeeperBottomLeft, -94.f, 133.f};
    ShapeKeeperHealthBar bottomLeftHealthBar {
        Art::instance().shapeKeeperHealthBarBottomLeft,
        {coreHealthBar.getPosition().x - 180, coreHealthBar.getPosition().y}
    };

    ShapeKeeperBodyPart bottomRight {Art::instance().shapeKeeperBottomRight, 104.f, 133.f};
    ShapeKeeperHealthBar bottomRightHealthBar {
        Art::instance().shapeKeeperHealthBarBottomRight,
        {coreHealthBar.getPosition().x - 90.f, coreHealthBar.getPosition().y}
    };

public:
    ShapeKeeper();

    static ShapeKeeper &instance()
    {
        static auto *instance = new ShapeKeeper;
        return *instance;
    }

    void update();
    void draw();
    void drawFont();
};



#endif //SHAPEKEEPER_H
