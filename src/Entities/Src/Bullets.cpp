#include "../Include/Bullets.h"
#include "../../GameRoot.h"
#include "../../System/Include/Extensions.h"
#include "../../System/Include/Quaternion.h"

sf::Vector2f Bullets::Bullet::getPosition() const {
    return sprite.getPosition();
}

void Bullets::Bullet::reset() {
    sprite.setPosition({0.0, 0.0});
    sprite.setRotation(sf::Angle::Zero);
    velocity = {0.0, 0.0};
    isActive = false;
}

void Bullets::incrementNextBulletIndex() {
    nextBulletIndex += 1;

    // Reset the bullet index to 0 if it is at the end of the array
    if (nextBulletIndex == bullets.size() - 1)
        nextBulletIndex = 0;
}


void Bullets::addBulletGroup(const sf::Vector2f fromPosition, const sf::Vector2f aimDirection) {

    // Create a random spread so the bullets mimic a machine gun like effect
    // Use a quaternion to rotate the initial position of the bullets in the direction they're travelling
    const float randomSpread = spreadDistribution(randEngine) + spreadDistribution(randEngine);
    const float aimAngle = toAngle(aimDirection);
    const Quaternion aimQuat = Quaternion::createFromYawPitchRoll(0, 0, aimAngle);
    const sf::Vector2f velocity = fromPolar(aimAngle + randomSpread, 13.0);

    // Add a group of 2 bullets with a small amount of offset so they are parallel
    sf::Vector2f offset = transform({15, -8.0}, aimQuat);
    bullets.at(nextBulletIndex).velocity = velocity;
    bullets.at(nextBulletIndex).sprite.setPosition(fromPosition + offset);
    bullets.at(nextBulletIndex).isActive = true;

    incrementNextBulletIndex();

    offset = transform({15.0, 8.0}, aimQuat);
    bullets.at(nextBulletIndex).velocity = velocity;
    bullets.at(nextBulletIndex).sprite.setPosition(fromPosition + offset);
    bullets.at(nextBulletIndex).isActive = true;

    incrementNextBulletIndex();

}

void Bullets::update() {
    // Update each bullet position and check that it should still be active
    for (std::size_t i = 0; i < bullets.size(); ++i)
        if (bullets[i].isActive) {
            // Rotate the bullet in the direction of its velocity
            if (bullets[i].velocity.lengthSquared() > 0)
                bullets[i].sprite.setRotation(sf::radians(toAngle(bullets[i].velocity)));

            // Move the bullet by its velocity
            bullets[i].sprite.move(bullets[i].velocity);

            // Check that the bullet is still within screen bound, if not reset it
            if (!GameRoot::instance().windowRectangle.contains(bullets[i].getPosition()))
                bullets[i].reset();
        }
}

void Bullets::draw() const {
    for (std::size_t i = 0; i < bullets.size(); ++i)
        if (bullets[i].isActive)
            GameRoot::instance().renderWindow.draw(bullets[i].sprite);
}

