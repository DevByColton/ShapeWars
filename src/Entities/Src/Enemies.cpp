#include "../Include/Enemies.h"
#include <algorithm>
#include "../../GameRoot.h"
#include "../../Content/Sound.h"
#include "../../PlayerStatus/PlayerStatus.h"
#include "../../System/Include/Extensions.h"
#include "../Include/PlayerShip.h"
#include "SFML/Graphics/Text.hpp"


void Enemies::EnemyKillText::reset() {

    fadeTime = 0;
    fadeScale = 0;
    position = {0.0, 0.0};
    text.setString({});
    text.setFillColor({255, 255, 255, 0});
    isActive = false;

}

void Enemies::EnemyKillText::fadeIn() {

    if (fadeTime < baseFadeTime) {

        fadeTime += GameRoot::instance().deltaTime;
        fadeScale += GameRoot::instance().deltaTime;

        if (fadeTime < baseFadeTime) {

            // Fade
            auto a = static_cast<std::uint8_t>(255 * (1.0f - fadeTime / baseFadeTime));
            text.setFillColor({255, 255, 255, a});

            // Scale
            auto scale = static_cast<float>(1.10 * (1.0f - fadeScale / baseFadeScale));
            text.setScale({ scale, scale });
            return;
        }

        // Make sure it is not visible at this point
        text.setFillColor({255, 255, 255, 0});


    }

    // The text is at full fade, reset it
    reset();

}


Enemies::Enemy::Enemy() {
    // Set the sprite origin to the middle of the sprite. All enemies are the same size
    float spriteMiddleX = static_cast<float>(Art::instance().enemyPlaceholder.getSize().x / 2.0);
    float spriteMiddleY = static_cast<float>(Art::instance().enemyPlaceholder.getSize().y / 2.0);
    sprite.setOrigin({spriteMiddleX, spriteMiddleY});
}


sf::Vector2f Enemies::Enemy::getPosition() const {
    return sprite.getPosition();
}


void Enemies::Enemy::reset() {
    sprite.setColor(sf::Color::Transparent);
    sprite.setTexture(Art::instance().enemyPlaceholder);
    sprite.setPosition({0.0, 0.0});
    sprite.setRotation(sf::Angle::Zero);
    velocity = {0.0, 0.0};
    timeUntilAct = maxTimeUntilAct;
    isActive = false;
}


float Enemies::Enemy::halfWidth() const {
    return spriteSizeF.x / 2;
}


float Enemies::Enemy::halfHeight() const {
    return spriteSizeF.y / 2;
}


bool Enemies::Enemy::canAct() {

    // Update the alpha of the sprite to fade in
    if (timeUntilAct > 0) {

        timeUntilAct -= GameRoot::instance().deltaTime;

        if (timeUntilAct > 0) {
            auto a = static_cast<std::uint8_t>(255 * (1.0f - timeUntilAct / maxTimeUntilAct));
            sprite.setColor({255, 255, 255, a});
            return false;
        }

        // Make sure the sprite is fully visible at this point
        sprite.setColor(sf::Color::White);
    }

    return true;

}


void Enemies::Enemy::killAddPoint() {

    PlayerStatus::instance().addPoints(pointValue);
    PlayerStatus::instance().increaseMultiplier();

    // Create a kill text at the position
    instance().addKillText(pointValue * PlayerStatus::instance().multiplier, getPosition());

    Sound::instance().playExplosionSound();

    // Reset the enemy
    reset();

}


void Enemies::Enemy::killNoPoints() {

    // Particles when I get to it

    // Reset the enemy
    reset();
}



void Enemies::addKillText(const int amount, const sf::Vector2f position) {

    EnemyKillText &enemyKillText = enemyKillTexts.at(nextKillTextIndex);

    enemyKillText.text.setString(std::to_string(amount));
    enemyKillText.text.setPosition(position);
    enemyKillText.isActive = true;

    nextKillTextIndex += 1;

    // Reset the next enemy text index is at 0
    if (nextKillTextIndex == enemyKillTexts.size() - 1)
        nextKillTextIndex = 0;

}



void Enemies::Enemy::pushApartBy(const Enemy &other) {

    const sf::Vector2f distance = getPosition() - other.getPosition();
    velocity += 20.0f * distance / (distance.lengthSquared() + 1);

}


void Enemies::incrementNextEnemyIndex() {

    nextEnemyIndex += 1;

    // Reset the enemy index to 0 if it is at the end of the array
    if (nextEnemyIndex == enemies.size() - 1)
        nextEnemyIndex = 0;

}


sf::Vector2f Enemies::createSpawnPosition() {

    sf::Vector2f position;

    // Create a new position and make sure it is not within the radius of the player to avoid spawning on top of them
    do {
        position = {xPositionDistribution(randEngine), yPositionDistribution(randEngine)};
    } while (distanceSquared(position, PlayerShip::instance().getPosition()) < 250.0 * 250.0);

    return position;

}


void Enemies::spawnSeeker() {

    // Create random starting position for the wanderer to start at
    const sf::Vector2f position = createSpawnPosition();

    // Create the seeker at the next index
    Enemy &seeker = enemies.at(nextEnemyIndex);
    seeker.sprite.setTexture(Art::instance().seeker);
    seeker.sprite.setPosition(position);
    seeker.speed = 1.05f;
    seeker.radius = 20;
    seeker.pointValue = 10;

    // Seek out the player by following
    seeker.behavior = [&seeker] () mutable {
        const float direction = toAngle(PlayerShip::instance().getPosition() - seeker.getPosition());
        seeker.velocity += fromPolar(direction, seeker.speed);

        // Move the enemy and make sure they are clamped into the screen
        const sf::Vector2f nextPosition = seeker.getPosition() + seeker.velocity;
        float clampedX = std::clamp(nextPosition.x, seeker.halfWidth(), GameRoot::instance().windowSizeF.x - seeker.halfWidth());
        float clampedY = std::clamp(nextPosition.y, seeker.halfHeight(), GameRoot::instance().windowSizeF.y - seeker.halfHeight());
        seeker.sprite.setPosition({clampedX, clampedY});

        // Rotation the enemy in the direction of its velocity
        if (seeker.velocity.lengthSquared() > 0)
            seeker.sprite.setRotation(sf::radians(toAngle(seeker.velocity)));

        seeker.velocity *= 0.9f;
    };

    // Activate the seeker
    seeker.isActive = true;

    incrementNextEnemyIndex();
    Sound::instance().playSpawnSound();

}

void Enemies::spawnWanderer() {

    // Create random starting position for the wanderer to start at
    const sf::Vector2f position = createSpawnPosition();

    // Create a wanderer at the next index
    Enemy &wanderer = enemies.at(nextEnemyIndex);
    wanderer.sprite.setTexture(Art::instance().wanderer);
    wanderer.sprite.setPosition(position);
    wanderer.speed = 1.1f;
    wanderer.radius = 20;
    wanderer.pointValue = 5;

    // Set the wanderer in a random direction
    wanderer.velocity = fromPolar(directionDistribution(randEngine), wanderer.speed);

    // Make the wanderer... wander in a random direction
    wanderer.behavior = [&wanderer] () mutable {
        const float direction = toAngle(wanderer.velocity);
        wanderer.velocity += fromPolar(direction, wanderer.speed);

        // Just rotate the sprite iteratively
        wanderer.sprite.rotate(sf::radians(-0.075));

        // Move the wanderer, and check if it is at the screen bounds
        const sf::Vector2f nextPosition = wanderer.getPosition() + wanderer.velocity;
        const bool isNextXOutOfBounds = nextPosition.x < wanderer.halfWidth() || nextPosition.x > GameRoot::instance().windowSizeF.x - wanderer.halfWidth();
        const bool isNextYOutOfBounds = nextPosition.y < wanderer.halfHeight() || nextPosition.y > GameRoot::instance().windowSizeF.y - wanderer.halfHeight();

        // If is at screen bounds bounce in a direction perpendicular of its current velocity
        if (isNextXOutOfBounds || isNextYOutOfBounds) {
            // Clamp the values and set the new position
            float clampedX = std::clamp(nextPosition.x, wanderer.halfWidth(), GameRoot::instance().windowSizeF.x - wanderer.halfWidth());
            float clampedY = std::clamp(nextPosition.y, wanderer.halfHeight(), GameRoot::instance().windowSizeF.y - wanderer.halfHeight());
            wanderer.sprite.setPosition({clampedX, clampedY});

            // Set the velocity perpendicular to its current velocity
            wanderer.velocity = {wanderer.velocity.y, -wanderer.velocity.x};
        } else {
            wanderer.sprite.setPosition(nextPosition);
        }

        // Basic friction
        wanderer.velocity *= 0.75f;
    };

    // Activate the wanderer
    wanderer.isActive = true;

    incrementNextEnemyIndex();
    Sound::instance().playSpawnSound();

}


void Enemies::killAll() {

    // Reset the spawn chance
    spawnChance = 60.0f;

    // Kill all the enemies
    for (std::size_t i = 0; i < enemies.size(); i++)
        enemies.at(i).killNoPoints();

    // Reset all the kill texts
    for (std::size_t i = 0; i < enemyKillTexts.size(); i++)
        enemyKillTexts.at(i).reset();

    // Reset iterators back to 0
    nextEnemyIndex = 0;
    nextKillTextIndex = 0;

}


void Enemies::update() {

    // Spawn new enemies
    std::uniform_real_distribution spawnDistribution {0.0f, spawnChance};

    if (static_cast<int>(spawnDistribution(randEngine)) == 0)
        spawnSeeker();

    if (static_cast<int>(spawnDistribution(randEngine)) == 0)
        spawnWanderer();

    // Reduce the spawn chance until it becomes 1 in 20
    if (spawnChance > 20.0)
        spawnChance -= GameRoot::instance().deltaTime;

    // Update all the enemies
    for (std::size_t i = 0; i < enemies.size(); ++i) {

        Enemy &enemy = enemies.at(i);

        if (enemy.isActive && enemy.canAct())
            enemy.behavior();

    }

    // Update all the active kill text
    for (std::size_t i = 0; i < enemyKillTexts.size(); i++) {

        EnemyKillText &enemyKillText = enemyKillTexts.at(i);

        if (enemyKillText.isActive)
            enemyKillText.fadeIn();

    }

}

void Enemies::draw() {

    // Draw all the enemies
    for (std::size_t i = 0; i < enemies.size(); ++i) {

        Enemy &enemy = enemies.at(i);

        if (enemy.isActive)
            GameRoot::instance().renderWindow.draw(enemy.sprite);
    }

    // Draw all the kill texts
    for (std::size_t i = 0; i < enemyKillTexts.size(); i++) {

        EnemyKillText &enemyKillText = enemyKillTexts.at(i);

        if (enemyKillText.isActive)
            GameRoot::instance().renderWindow.draw(enemyKillText.text);

    }
}





