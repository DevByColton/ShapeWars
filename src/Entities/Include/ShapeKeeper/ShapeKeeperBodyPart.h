#ifndef SHAPEKEEPERBODYPART_H
#define SHAPEKEEPERBODYPART_H
#include "ShapeKeeperCore.h"
#include "SFML/Graphics/Sprite.hpp"


struct ShapeKeeperHealthBar;

struct ShapeKeeperBodyPart final : sf::Sprite
{
    explicit ShapeKeeperBodyPart(const sf::Texture& texture, const sf::Texture& trailTexture, const sf::Color& color,
                                 ShapeKeeperHealthBar* healthBar, ShapeKeeperCore* core, float coreXOffset, float coreYOffset);

    static constexpr int MAX_HEALTH = 100;
    bool wasHit = false;
    bool hasBeenHitByNuke = false;
    int health = MAX_HEALTH;
    float coreXOffset = 0.f;
    float coreYOffset = 0.f;
    float radius = 0.f;
    int lastHitAmount = 0;
    sf::Vector2f positionRelativeToCore {0.f, 0.f};
    sf::Vector2f lastHitPosition = getPosition();
    sf::Color color {sf::Color::Transparent};
    ShapeKeeperCore* core {nullptr};
    ShapeKeeperHealthBar* healthBar {nullptr};
    sf::Sprite trailSprite;
    SpriteTrail spriteTrail {&trailSprite};

    void reset();
    bool isAlive() const;
    void markForHit(const sf::Vector2f &hitPosition, int amount);
    void update();
    void updateHealth();
    void updatePositionRelativeToCore();
    void draw();
};



#endif //SHAPEKEEPERBODYPART_H
