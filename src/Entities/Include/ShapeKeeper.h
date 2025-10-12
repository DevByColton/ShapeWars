#ifndef SHAPEKEEPER_H
#define SHAPEKEEPER_H
#include <random>
#include "../../GameRoot.h"
#include "../../Content/Include/Art.h"
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/RenderTexture.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Text.hpp"


class ShapeKeeper {
private:
    struct ShapeKeeperHealthBar final: sf::Sprite
    {
        explicit ShapeKeeperHealthBar(const sf::Texture& texture, const sf::Vector2f &position);

        sf::Sprite background;

        void update(int health);
    };

    struct ShapeKeeperCore final : sf::Sprite
    {
        explicit ShapeKeeperCore(const sf::Texture& texture, const sf::Color& color);

        bool wasHit = false;
        bool hasBeenHitByNuke = false;
        int health = BODY_PART_MAX_HEALTH;
        float radius = 0.f;
        float shieldRadius = 0.f;
        int lastHitAmount = 0;
        sf::Vector2f lastHitPosition = getPosition();
        sf::Color color {sf::Color::Transparent};
        sf::Sprite shield {Art::instance().shapeKeeperCoreShield};
        ShapeKeeperHealthBar* healthBar {nullptr};

        bool isAlive() const;
        void markForHit(const sf::Vector2f &hitPosition, int amount);
        void update();
    };

    struct ShapeKeeperBodyPart final : sf::Sprite
    {
        explicit ShapeKeeperBodyPart(const sf::Texture& texture, const sf::Color& color, float coreXOffset, float coreYOffset);

        bool wasHit = false;
        bool hasBeenHitByNuke = false;
        int health = BODY_PART_MAX_HEALTH;
        float coreXOffset = 0.f;
        float coreYOffset = 0.f;
        float radius = 0.f;
        int lastHitAmount = 0;
        sf::Vector2f lastHitPosition = getPosition();
        sf::Color color {sf::Color::Transparent};
        ShapeKeeperCore* core {nullptr};
        ShapeKeeperHealthBar* healthBar {nullptr};

        bool isAlive() const;
        void markForHit(const sf::Vector2f &hitPosition, int amount);
        void update();
    };

    static constexpr int BODY_PART_MAX_HEALTH = 35;
    sf::Text shapeKeeperText {Art::instance().shapeKeeperFont, {"SHAPE KEEPER"}, 60};
    sf::Sprite shapeKeeperHealthFrame {Art::instance().shapeKeeperHealthFrame};
    sf::RenderTexture healthWrapper {{shapeKeeperHealthFrame.getTexture().getSize().x, shapeKeeperHealthFrame.getTexture().getSize().y}};
    sf::Sprite healthWrapperSprite {healthWrapper.getTexture()};

    ShapeKeeperHealthBar coreHealthBar {
        Art::instance().shapeKeeperHealthBarCore,
        {shapeKeeperHealthFrame.getTexture().getSize().x - 65.f, 60.f}
    };

    ShapeKeeperHealthBar topHealthBar {
        Art::instance().shapeKeeperHealthBarTop,
        {coreHealthBar.getPosition().x - 450.f, coreHealthBar.getPosition().y}
    };

    ShapeKeeperHealthBar middleLeftHealthBar {
        Art::instance().shapeKeeperHealthBarMiddleLeft,
        {coreHealthBar.getPosition().x - 360.f, coreHealthBar.getPosition().y}
    };

    ShapeKeeperHealthBar middleRightHealthBar {
        Art::instance().shapeKeeperHealthBarMiddleRight,
        {coreHealthBar.getPosition().x - 270.f, coreHealthBar.getPosition().y}
    };

    ShapeKeeperHealthBar bottomLeftHealthBar {
        Art::instance().shapeKeeperHealthBarBottomLeft,
        {coreHealthBar.getPosition().x - 180, coreHealthBar.getPosition().y}
    };

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

    ShapeKeeperBodyPart top {Art::instance().shapeKeeperTop, {255, 233, 38, 255}, -8.f, -167.f};
    ShapeKeeperBodyPart middleLeft {Art::instance().shapeKeeperMiddleLeft, {53, 255, 203, 255}, -167.f, -49.f};
    ShapeKeeperBodyPart middleRight {Art::instance().shapeKeeperMiddleRight, {48, 255, 53, 255}, 154.f, -49.f};
    ShapeKeeperBodyPart bottomLeft {Art::instance().shapeKeeperBottomLeft, {255, 156, 25, 255}, -105.f, 133.f};
    ShapeKeeperBodyPart bottomRight {Art::instance().shapeKeeperBottomRight, {241, 38, 255, 255}, 93.f, 133.f};
    ShapeKeeperCore core {Art::instance().shapeKeeperCore, {255, 51, 113, 255}};

    bool canTakeCoreDamage() const;
    void update();
    void draw();
    void drawFont();
};



#endif //SHAPEKEEPER_H
