#include "../Include/Enemies.h"
#include <algorithm>
#include "../../GameRoot.h"
#include "../../Content/Include/GaussianBlur.h"
#include "../../Content/Include/Sound.h"
#include "../../Particles/Particles.h"
#include "../../PlayerStatus/Include/Buffs.h"
#include "../../PlayerStatus/Include/PlayerStatus.h"
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


void Enemies::Enemy::setLeading(Enemy* leading)
{
    this->leading = leading;
}


void Enemies::Enemy::setTrailing(Enemy* trailing)
{
    this->trailing = trailing;
}


sf::Vector2f Enemies::Enemy::getPosition() const
{
    return sprite.getPosition();
}


sf::Vector2f Enemies::Enemy::getCurrentVelocity() const
{
    return {xVelocity, yVelocity};
}


sf::Vector2f Enemies::Enemy::getTurnVelocity() const
{
    return {turnXVelocity, turnYVelocity};
}


bool Enemies::Enemy::isSnakeType() const
{
    return enemyType == SnakeHead || enemyType == SnakeBody || enemyType == SnakeTail;
}


bool Enemies::Enemy::isSnakeBody() const
{
    return enemyType == SnakeBody || enemyType == SnakeTail;
}


void Enemies::Enemy::reset(const bool canDropBuffChance)
{
    // Don't spawn particles if the enemy was not fully spawned
    // This case happens if the player dies during a spawn animation
    if (isActing)
    {
        const float hue1 = ColorPicker::instance().generateHue();
        const float hue2 = ColorPicker::instance().generateShiftedHue(hue1);
        const sf::Color color1 = ColorPicker::instance().hsvToRgb(hue1, 0.7f, 1.f);
        const sf::Color color2 = ColorPicker::instance().hsvToRgb(hue2, 0.7f, 1.f);

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
    }

    if (canDropBuffChance)
        Buffs::instance().checkBuffDrop(sprite.getPosition());

    sprite.setColor(sf::Color::White);
    sprite.setTexture(Art::instance().enemyPlaceholder);
    sprite.setPosition({0.0, 0.0});
    sprite.setRotation(sf::Angle::Zero);
    behavior = [] {};
    enemyType = None;
    isActive = false;
    isActing = false;
    shouldKill = false;
    trailing = nullptr;
    leading = nullptr;
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
    const auto alpha = static_cast<std::uint8_t>(255 * (1.f - timeUntilAct));
    sprite.setColor({255, 255, 255, alpha});
    sprite.setScale({1.f - timeUntilAct, 1.f - timeUntilAct});

    return false;
}


void Enemies::Enemy::markForKill()
{
    // Only mark should kill if it was not already marked this frame, and mark links
    if (!shouldKill)
    {
        shouldKill = true;

        if (trailing != nullptr)
            trailing->markForKill();

        if (leading != nullptr)
            leading->markForKill();
    }
}


void Enemies::Enemy::killAddPoints()
{
    if (pointValue != 0)
        FloatingKillTexts::instance().add(pointValue * PlayerStatus::instance().multiplier, getPosition());

    PlayerStatus::instance().addPoints(pointValue);
    PlayerStatus::instance().increaseMultiplier();
    Sound::instance().playExplosionSound();
    reset(true);
}


void Enemies::killAll()
{
    // Kill all the enemies
    for (int i = 0; i < enemies.size(); i++)
        if (enemies.at(i).isActive)
            enemies.at(i).reset(false);

    resetSpawnChances();
    resetEnemyPool();
}


void Enemies::Enemy::pushApartBy(const Enemy &other)
{
    // if (!isSnakeBody() && !(enemyType == SnakeHead && other.isSnakeBody()))
    if (!isSnakeType())
    {
        const sf::Vector2f distance = getPosition() - other.getPosition();
        const sf::Vector2f amount = 20.f * distance / (distance.lengthSquared() + 1);
        xVelocity += amount.x;
        yVelocity += amount.y;
    }
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
    sprite.setOrigin({sprite.getTexture().getSize().x / 2.f, sprite.getTexture().getSize().y / 2.f});
    radius = 20;
    timeUntilAct = maxTimeUntilAct;
    speed = 1.f;
    pointValue = 5;
    xVelocity = 0.0;
    yVelocity = 0.0;
    enemyType = Seeker;

    behavior = [this] () mutable
    {
        sf::Vector2f velocity = getCurrentVelocity();
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
    sprite.setOrigin({sprite.getTexture().getSize().x / 2.f, sprite.getTexture().getSize().y / 2.f});
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
        sf::Vector2f velocity = getCurrentVelocity();
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
    sprite.setOrigin({sprite.getTexture().getSize().x / 2.f, sprite.getTexture().getSize().y / 2.f});
    radius = sprite.getTexture().getSize().x / 2.f;
    timeUntilAct = maxTimeUntilAct;
    speed = 0.8f;
    pointValue = 7;
    xVelocity = 0.0;
    yVelocity = 0.0;
    enemyType = Dodger;

    behavior = [this] () mutable
    {
        sf::Vector2f velocity = getCurrentVelocity();
        const float direction = Extensions::toAngle(PlayerShip::instance().getPosition() - getPosition());
        velocity += Extensions::fromPolar(direction, speed);

        // Move the dodger and make sure it is clamped into the screen
        const sf::Vector2f nextPosition = getPosition() + velocity;
        float clampedX = std::clamp(nextPosition.x, halfWidth(), GameRoot::instance().windowSizeF.x - halfWidth());
        float clampedY = std::clamp(nextPosition.y, halfHeight(), GameRoot::instance().windowSizeF.y - halfHeight());
        sprite.setPosition({clampedX, clampedY});

        // Update the scale so the dodgers pulse slightly
        float scaleX = 1.f + 0.1f * std::cos(7.f * GameRoot::instance().elapsedGameTime);
        float scaleY = 1.f + 0.1f * std::sin(12.f * GameRoot::instance().elapsedGameTime);
        sprite.setScale({scaleX, scaleY});

        velocity *= 0.9f;
        xVelocity = velocity.x;
        yVelocity = velocity.y;
    };

    isActive = true;
    Sound::instance().playSpawnSound();
}


void Enemies::Enemy::activateSnakeHead()
{
    sprite.setTexture(Art::instance().snakeHead);
    sprite.setPosition(SpawnHelper::instance().createSpawnPosition());
    sprite.setOrigin({sprite.getTexture().getSize().x / 2.f, sprite.getTexture().getSize().y / 2.f});
    radius = sprite.getTexture().getSize().y / 2.f;
    timeUntilAct = maxTimeUntilAct * 0.5f;
    maxTimeUntilNewDirection = 0.75f;
    timeUntilNewDirection = maxTimeUntilNewDirection;
    speed = 1.f;
    pointValue = 10;
    const sf::Vector2f startingDirection = Extensions::fromPolar(instance().directionDistribution(instance().randEngine), speed);
    xVelocity = startingDirection.x;
    yVelocity = startingDirection.y;
    enemyType = SnakeHead;

    behavior = [this] () mutable
    {
        sf::Vector2f velocity = getCurrentVelocity();

        timeUntilNewDirection -= GameRoot::instance().deltaTime;
        if (timeUntilNewDirection < 0.f)
        {
            timeUntilNewDirection = maxTimeUntilNewDirection;
            const sf::Vector2f turnVelocity = Extensions::fromPolar(instance().directionDistribution(instance().randEngine), speed);
            turnXVelocity = turnVelocity.x;
            turnYVelocity = turnVelocity.y;
        }

        if (getTurnVelocity().lengthSquared() < 0.0001f)
        {
            turnXVelocity = 0.f;
            turnYVelocity = 0.f;
        }
        else
        {
            turnXVelocity *= 0.98f;
            turnYVelocity *= 0.98f;
        }

        const float direction = Extensions::toAngle(velocity - getTurnVelocity());
        velocity += Extensions::fromPolar(direction, speed);

        // Rotation the snake head in the direction of its velocity
        if (velocity.lengthSquared() > 0)
            sprite.setRotation(sf::radians(Extensions::toAngle(velocity)));

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


void Enemies::Enemy::activateSnakeBodyPart(const EnemyType enemyType)
{
    sprite.setTexture(enemyType == SnakeBody ? Art::instance().snakeBody : Art::instance().snakeTail);
    sprite.setPosition(leading->getPosition());
    sprite.setOrigin({sprite.getTexture().getSize().x / 2.f, sprite.getTexture().getSize().y / 2.f});
    radius = sprite.getTexture().getSize().x / 2.f;
    timeUntilAct = leading->timeUntilAct;
    timeUntilNewDirection = leading->timeUntilNewDirection;
    speed = leading->speed;
    pointValue = 0;
    xVelocity = 0.f;
    yVelocity = 0.f;
    this->enemyType = enemyType;

    behavior = [this] () mutable
    {
        sf::Vector2f velocity = getCurrentVelocity();
        const float direction = Extensions::toAngle(leading->getPosition() - getPosition());
        velocity += Extensions::fromPolar(direction, speed);

        // If there is enough distance between the links then allow movement, plus some padding
        if (Extensions::distanceSquared(leading->getPosition(), getPosition()) > radius * radius)
            sprite.move(velocity);

        // Rotate
        if (velocity.lengthSquared() > 0)
            sprite.setRotation(sf::radians(Extensions::toAngle(velocity)));

        // Basic friction
        velocity *= 0.8f;
        xVelocity = velocity.x;
        yVelocity = velocity.y;
    };

    isActive = true;
}


void Enemies::checkSeekerSpawn()
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


void Enemies::checkWandererSpawn()
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


void Enemies::checkDodgerSpawn()
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


void Enemies::checkSnakeSpawn()
{
    // Decrease the spawn chance every frame, until its about 1 in 50
    if (snakeSpawnChance > 50.f)
        snakeSpawnChance -= GameRoot::instance().deltaTime;

    // Roll the dice
    std::uniform_real_distribution spawnChanceDistribution {0.f, snakeSpawnChance};
    if (static_cast<int>(spawnChanceDistribution(randEngine)) == 0)
    {
        assert(firstAvailable != nullptr);

        // Get the count of how long the snake would be, check to make sure there are enough available enemy slots
        const int bodyPartCount = snakeBodyPartCountDistribution(randEngine);
        const Enemy *availableEnemyCheck = firstAvailable;
        for (int i = 0; i < bodyPartCount + 2; i++)
        {
            if (availableEnemyCheck->getNext() != nullptr)
                availableEnemyCheck = availableEnemyCheck->getNext();
            else
                return;
        }

        // Create the snake head and first body part for linking
        Enemy *snakeHead = firstAvailable;
        firstAvailable = snakeHead->getNext();
        snakeHead->activateSnakeHead();

        Enemy *snakeBodyFirst = firstAvailable;
        firstAvailable = snakeBodyFirst->getNext();
        snakeHead->setTrailing(snakeBodyFirst);
        snakeBodyFirst->setLeading(snakeHead);
        snakeBodyFirst->activateSnakeBodyPart(SnakeBody);

        // Create the rest of the body of the snake and terminate with the tail
        Enemy *leadingSnakeBodyPart = snakeBodyFirst;
        for (int i = 0; i < bodyPartCount; i++)
        {
            if (i == bodyPartCount - 1)
            {
                Enemy *snakeTail = firstAvailable;
                firstAvailable = snakeTail->getNext();
                leadingSnakeBodyPart->setTrailing(snakeTail);
                snakeTail->setLeading(leadingSnakeBodyPart);
                snakeTail->activateSnakeBodyPart(SnakeTail);
                break;
            }

            Enemy *snakeBody = firstAvailable;
            firstAvailable = snakeBody->getNext();
            leadingSnakeBodyPart->setTrailing(snakeBody);
            snakeBody->setLeading(leadingSnakeBodyPart);
            snakeBody->activateSnakeBodyPart(SnakeBody);
            leadingSnakeBodyPart = snakeBody;
        }
    }
}



void Enemies::update()
{
    // Check spawn of new enemies
    if (canSpawn)
    {
        checkSeekerSpawn();
        checkWandererSpawn();
        checkDodgerSpawn();
        checkSnakeSpawn();
    }

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
    {
        // Draw an extra sprite for a spawning animation
        if (timeUntilAct > 0.f)
        {
            const auto alpha = static_cast<std::uint8_t>(255 - 255 * (1.f - timeUntilAct));
            sf::Sprite animatedSprite {sprite.getTexture()};
            animatedSprite.setPosition(sprite.getPosition());
            animatedSprite.setOrigin(sprite.getOrigin());
            animatedSprite.setColor({255, 255, 255, alpha});
            animatedSprite.setScale({
                (1.f - timeUntilAct) * 2.f,
                (1.f - timeUntilAct) * 2.f
            });
            GaussianBlur::instance().drawToBase(animatedSprite);
        }

        GaussianBlur::instance().drawToBase(sprite);
    }
}

