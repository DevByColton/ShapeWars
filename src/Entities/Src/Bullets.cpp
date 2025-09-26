#include "../Include/Bullets.h"
#include "../../GameRoot.h"
#include "../../Content/Include/GaussianBlur.h"
#include "../../Content/Include/Sound.h"
#include "../../Grid/Grid.h"
#include "../../Particles/Particles.h"
#include "../../System/Include/Extensions.h"
#include "../../System/Include/Quaternion.h"
#include "../../System/Include/RandomVector.h"


Bullets::Bullets()
{
    resetBulletPool();
}


void Bullets::resetBulletPool()
{
    // Set the head of the bullets
    firstAvailable = &bullets.at(0);

    // Link them up
    for (int i = 0; i < MAX_BULLET_COUNT - 1; i++)
        bullets.at(i).setNext(&bullets.at(i + 1));

    // The last one in the list terminates
    bullets.at(MAX_BULLET_COUNT - 1).setNext(nullptr);
}


Bullets::Bullet* Bullets::Bullet::getNext() const
{
    return next;
}


void Bullets::Bullet::setNext(Bullet *next)
{
    this->next = next;
}


Bullets::Bullet::Bullet()
{
    const float x = sprite.getTexture().getSize().x / 2;
    const float y = sprite.getTexture().getSize().y / 2;
    sprite.setOrigin({x , y});
    radius = x;
}


sf::Vector2f Bullets::Bullet::getPosition() const
{
    return sprite.getPosition();
}


sf::Vector2f Bullets::Bullet::getVelocity() const
{
    return {xVelocity, yVelocity};
}


void Bullets::Bullet::applyForce(const sf::Vector2f amount)
{
    xVelocity += amount.x;
    yVelocity += amount.y;
}



void Bullets::Bullet::markForBlowUp()
{
    // Only mark should blow up if not already marked this frame
    if (!shouldBlowUp)
        shouldBlowUp = true;
}


bool Bullets::Bullet::getShouldBlowUp() const
{
    return shouldBlowUp;
}


void Bullets::Bullet::blowUp()
{
    for (int i = 0; i < 30; i++)
        Particles::instance().create(
            1.f,
            DontIgnoreGravity,
            i % 4 == 0 ? Explosion : Spark,
            sprite.getPosition(),
            RandomVector::instance().next(0.5f, 8.f),
            instance().bulletExplosionColor
        );

    sprite.setPosition({0.0, 0.0});
    sprite.setRotation(sf::Angle::Zero);
    xVelocity = 0.0;
    yVelocity = 0.0;
    isActive = false;
    shouldBlowUp = false;
}


void Bullets::Bullet::activate(const sf::Vector2f velocity, const sf::Vector2f position)
{
    xVelocity = velocity.x;
    yVelocity = velocity.y;
    sprite.setPosition(position);
    isActive = true;
}


void Bullets::addBulletGroup(const sf::Vector2f fromPosition, const sf::Vector2f aimDirection)
{
    assert(firstAvailable != nullptr);

    // Make sure 2 bullets are available
    Bullet *bullet = firstAvailable;
    if (bullet->getNext() != nullptr && bullet->getNext()->getNext() != nullptr)
    {
        firstAvailable = bullet->getNext();
        Bullet *bullet2 = firstAvailable;
        firstAvailable = bullet2->getNext();

        // Create a random spread so the bullets mimic a machine gun like effect
        // Use a quaternion to rotate the initial position of the bullets in the direction they're travelling
        const float randomSpread = spreadDistribution(randEngine) + spreadDistribution(randEngine);
        const float aimAngle = Extensions::toAngle(aimDirection);
        const Quaternion aimQuat = Quaternion::createFromYawPitchRoll(0, 0, aimAngle);
        const sf::Vector2f velocity = Extensions::fromPolar(aimAngle + randomSpread, 20.0);

        // Add a group of 2 bullets with a small amount of offset so they are parallel
        sf::Vector2f offset = Extensions::transform({15, -8.0}, aimQuat);
        bullet->activate(velocity, fromPosition + offset);
        offset = Extensions::transform({15.0, 8.0}, aimQuat);
        bullet2->activate(velocity, fromPosition + offset);

        Sound::instance().playShotSound();
    }
}


void Bullets::resetAll()
{
    // Blow up all the bullets
    for (int i = 0; i < bullets.size(); i++)
        if (bullets.at(i).isActive)
            bullets.at(i).blowUp();

    resetBulletPool();
}


void Bullets::update()
{
    // Update each bullet position and check that it should still be active
    for (int i = 0; i < MAX_BULLET_COUNT; i++)
        if (bullets.at(i).isActive)
            if (bullets.at(i).getShouldBlowUp() || !GameRoot::instance().windowRectangle.contains(bullets.at(i).getPosition()))
            {
                bullets.at(i).blowUp();
                bullets.at(i).setNext(firstAvailable);
                firstAvailable = &bullets.at(i);
            }
            else
            {
                bullets.at(i).update();
            }
}


void Bullets::Bullet::update()
{
    // Rotate the bullet in the direction of its velocity
    const sf::Vector2f velocity {xVelocity, yVelocity};

    if (velocity.lengthSquared() > 0)
        sprite.setRotation(sf::radians(Extensions::toAngle(velocity)));

    // Move the bullet by its velocity
    sprite.move(velocity);

    // Make some waves on the grid
    Grid::instance().applyExplosiveForce(getPosition(), 0.5f * velocity.length(), 80.f, 0.4f);
}


void Bullets::draw() const
{
    for (int i = 0; i < bullets.size(); ++i)
        bullets.at(i).draw();
}


void Bullets::Bullet::draw() const
{
    if (isActive)
        GaussianBlur::instance().drawToBase(sprite);
}


