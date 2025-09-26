#include "../Include/Enemies.h"
#include <algorithm>
#include "../../GameRoot.h"
#include "../../Content/Include/GaussianBlur.h"
#include "../../Content/Include/Sound.h"
#include "../../Particles/Particles.h"
#include "../../PlayerStatus/PlayerStatus.h"
#include "../../System/Include/ColorPicker.h"
#include "../../System/Include/Extensions.h"
#include "../../System/Include/RandomVector.h"
#include "../../System/Include/SpawnHelper.h"
#include "../../UserInterface/Include/FloatingKillTexts.h"
#include "../Include/PlayerShip.h"
#include "SFML/Graphics/Text.hpp"

Enemies::Enemies()
{
    resetEnemyPool();
    resetSpawnChances();
}


void Enemies::resetEnemyPool()
{
    // Set the head of the enemies
    firstAvailable = &enemies.at(0);

    // Link them up
    for (int i = 0; i < MAX_ENEMY_COUNT - 1; i++)
        enemies.at(i).setNext(&enemies.at(i + 1));

    // The last one in the list terminates
    enemies.at(MAX_ENEMY_COUNT - 1).setNext(nullptr);
}


void Enemies::resetSpawnChances()
{
    seekerSpawnChance = 60.f;
    wandererSpawnChance = 60.f;
    dodgerSpawnChance = 100.f;
}


Enemies::Enemy* Enemies::Enemy::getNext() const
{
    return next;
}


void Enemies::Enemy::setNext(Enemy *next)
{
    this->next = next;
}


Enemies::Enemy::Enemy()
{
    // Set the sprite origin to the middle of the sprite. All enemies are the same size
    float x = Art::instance().enemyPlaceholder.getSize().x / 2;
    float y = Art::instance().enemyPlaceholder.getSize().y / 2;
    sprite.setOrigin({x, y});
    sprite.setColor(sf::Color::White);
}


sf::Vector2f Enemies::Enemy::getPosition() const
{
    return sprite.getPosition();
}


sf::Vector2f Enemies::Enemy::getVelocity() const
{
    return {xVelocity, yVelocity};
}


void Enemies::Enemy::reset()
{
    const float hue1 = ColorPicker::instance().generateHue();
    const float hue2 = ColorPicker::instance().generateShiftedHue(hue1);
    const sf::Color color1 = ColorPicker::instance().hsvToRgb(hue1, 0.5f, 1.f);
    const sf::Color color2 = ColorPicker::instance().hsvToRgb(hue2, 0.5f, 1.f);

    for (int i = 0; i < 120; i++)
    {
        const float speed = Particles::instance().randomStartingSpeed(15.0, 1.f, 10.f);
        Particles::instance().create(
            3.f,
            DontIgnoreGravity,
            Explosion,
            getPosition(),
            RandomVector::instance().next(speed, speed),
            ColorPicker::instance().lerp(color1, color2)
        );
    }

    sprite.setColor(sf::Color::White);
    sprite.setTexture(Art::instance().enemyPlaceholder);
    sprite.setPosition({0.0, 0.0});
    sprite.setRotation(sf::Angle::Zero);
    behavior = [] {};
    enemyType = None;
    isActive = false;
    isActing = false;
    shouldKill = false;
}


float Enemies::Enemy::halfWidth() const
{
    return spriteSizeF.x / 2;
}


float Enemies::Enemy::halfHeight() const
{
    return spriteSizeF.y / 2;
}


bool Enemies::Enemy::canAct()
{
    timeUntilAct -= GameRoot::instance().deltaTime;

    if (timeUntilAct <= 0.f)
    {
        // Make sure the sprite is fully visible at this point
        sprite.setColor(sf::Color::White);
        sprite.setScale({1.f, 1.f});
        isActing = true;
        return true;
    }

    // Update the spawning animation
    if (timeUntilAct < halfMaxTimeUntilAct)
    {
        // Grow bigger and fade out
        auto a = static_cast<std::uint8_t>(255 * (1.f - timeUntilAct / halfMaxTimeUntilAct));
        sprite.setColor({255, 255, 255, a});
        sprite.setScale({
            (timeUntilAct + halfMaxTimeUntilAct) * 1.5f,
            (timeUntilAct + halfMaxTimeUntilAct) * 1.5f
        });
    }
    else
    {
        // Now fade back in and bring the scale back down to about 1
        const auto b = static_cast<std::uint8_t>(255 * (timeUntilAct - halfMaxTimeUntilAct));
        sprite.setColor({255, 255, 255, b});
        sprite.setScale({
            1.5f - (timeUntilAct - halfMaxTimeUntilAct),
            1.5f - (timeUntilAct - halfMaxTimeUntilAct)
        });
    }

    return false;
}


void Enemies::Enemy::markForKill()
{
    // Only mark should kill if it was not already marked this frame
    if (!shouldKill)
        shouldKill = true;
}


void Enemies::Enemy::killAddPoints()
{
    PlayerStatus::instance().addPoints(pointValue);
    PlayerStatus::instance().increaseMultiplier();
    FloatingKillTexts::instance().add(pointValue * PlayerStatus::instance().multiplier, getPosition());
    Sound::instance().playExplosionSound();
    reset();
}


void Enemies::killAll()
{
    // Kill all the enemies
    for (int i = 0; i < enemies.size(); i++)
        if (enemies.at(i).isActive)
            enemies.at(i).reset();

    resetSpawnChances();
    resetEnemyPool();
}


void Enemies::Enemy::pushApartBy(const Enemy &other)
{
    const sf::Vector2f distance = getPosition() - other.getPosition();
    const sf::Vector2f amount = 20.f * distance / (distance.lengthSquared() + 1);
    xVelocity += amount.x;
    yVelocity += amount.y;
}


void Enemies::Enemy::applyForce(const sf::Vector2f velocity)
{
    xVelocity += velocity.x;
    yVelocity += velocity.y;
}


void Enemies::Enemy::activateSeeker()
{
    sprite.setTexture(Art::instance().seeker);
    sprite.setPosition(SpawnHelper::instance().createSpawnPosition());
    radius = 20;
    timeUntilAct = maxTimeUntilAct;
    speed = 1.f;
    pointValue = 5;
    xVelocity = 0.0;
    yVelocity = 0.0;
    enemyType = Seeker;

    behavior = [this] () mutable
    {
        sf::Vector2f velocity = getVelocity();
        const float direction = Extensions::toAngle(PlayerShip::instance().getPosition() - getPosition());
        velocity += Extensions::fromPolar(direction, speed);

        // Move the seeker and make sure it is clamped into the screen
        const sf::Vector2f nextPosition = getPosition() + velocity;
        float clampedX = std::clamp(nextPosition.x, halfWidth(), GameRoot::instance().windowSizeF.x - halfWidth());
        float clampedY = std::clamp(nextPosition.y, halfHeight(), GameRoot::instance().windowSizeF.y - halfHeight());
        sprite.setPosition({clampedX, clampedY});

        // Rotation the seeker in the direction of its velocity
        if (velocity.lengthSquared() > 0)
            sprite.setRotation(sf::radians(Extensions::toAngle(velocity)));

        velocity *= 0.9f;
        xVelocity = velocity.x;
        yVelocity = velocity.y;
    };

    isActive = true;
    Sound::instance().playSpawnSound();
}


void Enemies::Enemy::activateWanderer()
{
    sprite.setTexture(Art::instance().wanderer);
    sprite.setPosition(SpawnHelper::instance().createSpawnPosition());
    radius = 20;
    timeUntilAct = maxTimeUntilAct;
    speed = 1.f;
    pointValue = 3;
    const sf::Vector2f startingDirection = Extensions::fromPolar(instance().directionDistribution(instance().randEngine), speed);
    xVelocity = startingDirection.x;
    yVelocity = startingDirection.y;
    enemyType = Wanderer;

    behavior = [this] () mutable
    {
        sf::Vector2f velocity = getVelocity();
        const float direction = Extensions::toAngle(velocity);
        velocity += Extensions::fromPolar(direction, speed);

        // Just rotate the sprite iteratively
        sprite.rotate(sf::radians(0.1f));

        // Move the wanderer, and check if it is at the screen bounds
        const sf::Vector2f nextPosition = getPosition() + velocity;

        if (nextPosition.x < 0)
            velocity.x = std::abs(velocity.x);
        else if (nextPosition.x > GameRoot::instance().windowSizeF.x)
            velocity.x = -std::abs(velocity.x);

        if (nextPosition.y < 0)
            velocity.y = std::abs(velocity.y);
        else if (nextPosition.y > GameRoot::instance().windowSizeF.y)
            velocity.y = -std::abs(velocity.y);

        sprite.setPosition(nextPosition);

        // Basic friction
        velocity *= 0.8f;
        xVelocity = velocity.x;
        yVelocity = velocity.y;
    };

    isActive = true;
    Sound::instance().playSpawnSound();
}


void Enemies::Enemy::activateDodger()
{
    sprite.setTexture(Art::instance().dodger);
    sprite.setPosition(SpawnHelper::instance().createSpawnPosition());
    radius = 20;
    timeUntilAct = maxTimeUntilAct;
    speed = 1.f;
    pointValue = 7;
    xVelocity = 0.0;
    yVelocity = 0.0;
    enemyType = Dodger;

    behavior = [this] () mutable
    {
        sf::Vector2f velocity = getVelocity();
        const float direction = Extensions::toAngle(PlayerShip::instance().getPosition() - getPosition());
        velocity += Extensions::fromPolar(direction, speed);

        // Move the dodger and make sure it is clamped into the screen
        const sf::Vector2f nextPosition = getPosition() + velocity;
        float clampedX = std::clamp(nextPosition.x, halfWidth(), GameRoot::instance().windowSizeF.x - halfWidth());
        float clampedY = std::clamp(nextPosition.y, halfHeight(), GameRoot::instance().windowSizeF.y - halfHeight());
        sprite.setPosition({clampedX, clampedY});

        // Rotation the dodger in the direction of its velocity
        if (velocity.lengthSquared() > 0)
            sprite.setRotation(sf::radians(std::sin(6.f * GameRoot::instance().elapsedGameTime)));

        velocity *= 0.9f;
        xVelocity = velocity.x;
        yVelocity = velocity.y;
    };

    isActive = true;
    Sound::instance().playSpawnSound();
}


void Enemies::checkSpawnSeeker()
{
    // Decrease the spawn chance every frame, until its about 1 in 15
    if (seekerSpawnChance > 15.f)
        seekerSpawnChance -= GameRoot::instance().deltaTime;

    // Roll the dice
    std::uniform_real_distribution spawnChanceDistribution {0.f, seekerSpawnChance};
    if (static_cast<int>(spawnChanceDistribution(randEngine)) == 0)
    {
        assert(firstAvailable != nullptr);

        // Make sure it is not the last in the list
        Enemy *seeker = firstAvailable;
        if (seeker->getNext() != nullptr)
        {
            firstAvailable = seeker->getNext();
            seeker->activateSeeker();
        }
    }
}


void Enemies::checkSpawnWanderer()
{
    // Decrease the spawn chance every frame, until its about 1 in 20
    if (wandererSpawnChance > 20.f)
        wandererSpawnChance -= GameRoot::instance().deltaTime;

    // Roll the dice
    std::uniform_real_distribution spawnChanceDistribution {0.f, wandererSpawnChance};
    if (static_cast<int>(spawnChanceDistribution(randEngine)) == 0)
    {
        assert(firstAvailable != nullptr);

        // Make sure it is not the last in the list
        Enemy *wanderer = firstAvailable;
        if (wanderer->getNext() != nullptr)
        {
            firstAvailable = wanderer->getNext();
            wanderer->activateWanderer();
        }
    }
}


void Enemies::checkSpawnDodger()
{
    // Decrease the spawn chance every frame, until its about 1 in 30
    if (dodgerSpawnChance > 30.f)
        dodgerSpawnChance -= GameRoot::instance().deltaTime;

    // Roll the dice
    std::uniform_real_distribution spawnChanceDistribution {0.f, dodgerSpawnChance};
    if (static_cast<int>(spawnChanceDistribution(randEngine)) == 0)
    {
        assert(firstAvailable != nullptr);

        // Make sure it is not last in the list
        Enemy *dodger = firstAvailable;
        if (dodger->getNext() != nullptr)
        {
            firstAvailable = dodger->getNext();
            dodger->activateDodger();
        }
    }
}


void Enemies::update()
{
    // Check spawn of new enemies
    checkSpawnSeeker();
    checkSpawnWanderer();
    checkSpawnDodger();

    // Update all the enemies and check the kill status
    for (int i = 0; i < MAX_ENEMY_COUNT; i++)
        if (enemies.at(i).isActive)
            if (enemies.at(i).shouldKill)
            {
                // Kill and reset, then set enemy to the front of the list
                enemies.at(i).killAddPoints();
                enemies.at(i).setNext(firstAvailable);
                firstAvailable = &enemies.at(i);
            }
            else
            {
                enemies.at(i).update();
            }
}


void Enemies::Enemy::update()
{
    if (isActing)
        behavior();
    else
        canAct();
}


void Enemies::draw() const
{
    for (int i = 0; i < MAX_ENEMY_COUNT; i++)
        enemies.at(i).draw();
}


void Enemies::Enemy::draw() const
{
    if (isActive)
        GaussianBlur::instance().drawToBase(sprite);
}

