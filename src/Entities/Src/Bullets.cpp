#include "../Include/Bullets.h"
#include "../../GameRoot.h"
#include "../../Content/Sound.h"
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
    const sf::Vector2f velocity = fromPolar(aimAngle + randomSpread, 20.0);

    // Add a group of 2 bullets with a small amount of offset so they are parallel
    sf::Vector2f offset = transform({15, -8.0}, aimQuat);
    Bullet &bullet1 = bullets.at(nextBulletIndex);
    bullet1.velocity = velocity;
    bullet1.sprite.setPosition(fromPosition + offset);
    bullet1.isActive = true;

    incrementNextBulletIndex();

    Bullet &bullet2 = bullets.at(nextBulletIndex);
    offset = transform({15.0, 8.0}, aimQuat);
    bullet2.velocity = velocity;
    bullet2.sprite.setPosition(fromPosition + offset);
    bullet2.isActive = true;

    incrementNextBulletIndex();

    // Play shot sound
    Sound::instance().playShotSound();

}


void Bullets::resetAll() {

    for (std::size_t i = 0; i < bullets.size(); i++)
        bullets.at(i).reset();

    nextBulletIndex = 0;

}


void Bullets::update() {

    // Update each bullet position and check that it should still be active
    for (std::size_t i = 0; i < bullets.size(); ++i) {
        Bullet &bullet = bullets.at(i);
        if (bullet.isActive) {
            // Rotate the bullet in the direction of its velocity
            if (bullet.velocity.lengthSquared() > 0)
                bullet.sprite.setRotation(sf::radians(toAngle(bullet.velocity)));

            // Move the bullet by its velocity
            bullet.sprite.move(bullet.velocity);

            // Check that the bullet is still within screen bound, if not reset it
            if (!GameRoot::instance().windowRectangle.contains(bullet.getPosition()))
                bullet.reset();
        }
    }

}


void Bullets::draw() {

    for (std::size_t i = 0; i < bullets.size(); ++i) {
        Bullet &bullet = bullets.at(i);
        if (bullet.isActive)
            GameRoot::instance().renderWindow.draw(bullet.sprite);
    }

}

