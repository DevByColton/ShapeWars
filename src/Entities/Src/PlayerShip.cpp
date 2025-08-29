#include <algorithm>
#include "../Include/PlayerShip.h"
#include "../../GameRoot.h"
#include "../../Content/Art.h"
#include "../../Input/Input.h"
#include "../../System/Include/Extensions.h"
#include "../Include/Bullets.h"

PlayerShip::PlayerShip() {

    // Set the players position in the middle of the screen
    float x = GameRoot::instance().windowSizeF.x / 2;
    float y = GameRoot::instance().windowSizeF.y / 2;
    sprite.setPosition({x, y});

    // Set the sprite origin to the middle of the sprite
    float spriteMiddleX = static_cast<float>(Art::instance().Player.getSize().x / 2.0);
    float spriteMiddleY = static_cast<float>(Art::instance().Player.getSize().y / 2.0);
    sprite.setOrigin({spriteMiddleX, spriteMiddleY});

    // Set the size of the sprite in float
    float width = static_cast<float>(sprite.getTexture().getSize().x);
    float height = static_cast<float>(sprite.getTexture().getSize().y);
    spriteSizeF = {width, height};

    radius = 20;

    // Set the random generator
    std::uniform_int_distribution<int> dist(1, 100);
}

sf::Vector2f PlayerShip::getPosition() const {
    return sprite.getPosition();
}

sf::Vector2<int> PlayerShip::size() const {
    return sf::Vector2<int>(sprite.getTexture().getSize());
}

float PlayerShip::halfWidth() const {
    return spriteSizeF.x / 2;
}

float PlayerShip::halfHeight() const {
    return spriteSizeF.y / 2;
}

void PlayerShip::update() {
    // Move the player and clamp to window bounds
    velocity = speed * Input::instance().getMovementDirection();
    const sf::Vector2f nextPosition = sprite.getPosition() + velocity;
    float clampedX = std::clamp(nextPosition.x, halfWidth(), GameRoot::instance().windowSizeF.x - halfWidth());
    float clampedY = std::clamp(nextPosition.y, halfHeight(), GameRoot::instance().windowSizeF.y - halfHeight());
    sprite.setPosition({clampedX, clampedY});

    // Rotation the player ship in the direction of its velocity
    if (velocity.lengthSquared() > 0)
        sprite.setRotation(sf::radians(toAngle(velocity)));

    // Check aiming direction and fire bullets
    const sf::Vector2f aimDirection = Input::instance().getAimDirection();
    if (aimDirection.lengthSquared() > 0 && spawnBulletCooldownRemaining <= 0) {
        // Reset fire bullets cooldown
        spawnBulletCooldownRemaining = spawnBulletCooldownFrames;
        Bullets::instance().addBulletGroup(getPosition(), aimDirection);
    }

    // Decrement the bullet cooldown
    if (spawnBulletCooldownRemaining > 0)
        spawnBulletCooldownRemaining -= 1;
}

void PlayerShip::draw() const {
    GameRoot::instance().renderWindow.draw(sprite);
}

