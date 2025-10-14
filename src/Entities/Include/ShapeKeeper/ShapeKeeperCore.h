#ifndef SHAPEKEEPERCORE_H
#define SHAPEKEEPERCORE_H
#include "ShapeKeeperHealthBar.h"
#include "../../../Content/Include/Art.h"
#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/System/Vector2.hpp"


struct ShapeKeeperCore final : sf::Sprite
{
    explicit ShapeKeeperCore(const sf::Texture& texture, const sf::Color& color, ShapeKeeperHealthBar* healthBar);

    static constexpr int MAX_HEALTH = 30;
    bool wasHit = false;
    bool hasBeenHitByNuke = false;
    int health = MAX_HEALTH;
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
    void draw(bool canTakeCoreDamage) const;
};



#endif //SHAPEKEEPERCORE_H
