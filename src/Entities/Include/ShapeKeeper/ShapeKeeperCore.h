#ifndef SHAPEKEEPERCORE_H
#define SHAPEKEEPERCORE_H
#include <functional>
#include <random>
#include "ShapeKeeperHealthBar.h"
#include "SpriteTrail.h"
#include "../../../GameRoot.h"
#include "../../../Content/Include/Art.h"
#include "../../../Content/Include/GaussianBlur.h"
#include "../../../System/Include/Extensions.h"
#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/System/Vector2.hpp"


struct ShapeKeeperCore final : sf::Sprite
{
    explicit ShapeKeeperCore(const sf::Texture& texture, const sf::Color& color, ShapeKeeperHealthBar* healthBar);

    static constexpr int MAX_HEALTH = 30;
    const sf::Vector2f offScreenLeft {-400.f, GameRoot::instance().windowSizeF.y / 2.f};
    const sf::Vector2f offScreenRight {GameRoot::instance().windowSizeF.x + 400.f, GameRoot::instance().windowSizeF.y / 2.f};
    bool wasHit = false;
    bool hasBeenHitByNuke = false;
    int health = MAX_HEALTH;
    float radius = 0.f;
    float shieldRadius = 0.f;
    int lastHitAmount = 0;
    sf::Vector2f lastHitPosition = getPosition();
    sf::Color color {sf::Color::Transparent};
    sf::Sprite shield {Art::instance().shapeKeeperCoreShield};
    sf::Sprite trailSprite {Art::instance().shapeKeeperCoreTrail};
    SpriteTrail spriteTrail {&trailSprite};
    ShapeKeeperHealthBar* healthBar {nullptr};

    // Movement
    float timeUntilMovementChangeDuration = 0.f;
    float timeUntilMovementChange = 0.f;
    sf::Vector2f previousTargetPosition {0.f, 0.f};
    sf::Vector2f currentTargetPosition {0.f, 0.f};
    std::default_random_engine timeUntilMovementChangeRandEngine {std::random_device{}()};
    std::default_random_engine movementXRandEngine {std::random_device{}()};
    std::default_random_engine movementYRandEngine {std::random_device{}()};
    std::uniform_real_distribution<float> timeUntilMovementChangeDistribution {1.f, 5.f};
    std::uniform_real_distribution<float> movementXDistribution {200.f, GameRoot::instance().windowSizeF.x - 200.f};
    std::uniform_real_distribution<float> movementYDistribution {200.f, GameRoot::instance().windowSizeF.y - 200.f};

    // Rotation
    float timeUntilRotationChangeDuration = 0.f;
    float timeUntilRotationChange = 0.f;
    sf::Angle currentBodyPartRotation = sf::Angle::Zero;
    sf::Angle previousRotationTarget = currentBodyPartRotation;
    sf::Angle currentRotationTarget = currentBodyPartRotation;
    std::default_random_engine timeUntilRotationChangeRandEngine {std::random_device{}()};
    std::default_random_engine rotationChangeRandEngine {std::random_device{}()};
    std::uniform_real_distribution<float> timeUntilRotationChangeDistribution {1.f, 3.f};
    std::uniform_real_distribution<float> rotationChangeDistribution {0.f, TWO_PI};

    std::function<void()> onDeath;

    bool isAlive() const;
    void reset();
    void markForHit(const sf::Vector2f &hitPosition, int amount);
    void activate(bool isOnLeftSideOfScreen);
    void update();
    void updateHealth();
    void updateMovement();
    void updateRotation();
    void setMovementVariables();
    void draw(bool canTakeCoreDamage);
};



#endif //SHAPEKEEPERCORE_H
