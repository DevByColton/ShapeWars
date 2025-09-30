#include <fstream>
#include "PlayerStatus.h"
#include "../GameRoot.h"
#include "../Entities/Include/PlayerShip.h"
#include "../Grid/Grid.h"
#include "../Logger/Logger.h"
#include "../Particles/Particles.h"
#include "../System/Include/ColorPicker.h"
#include "../System/Include/Extensions.h"


PlayerStatus::PlayerStatus()
{
    loadHighscore();
    roundClock.restart();
}


void PlayerStatus::stopRoundClock()
{
    roundClock.stop();
}


void PlayerStatus::startRoundClock()
{
    roundClock.start();
}


void PlayerStatus::reset()
{
    // Check if highscore needs to be saved
    if (score > highScore) {
        saveHighscore(score);
        highScore = score;
    }

    score = 0;
    scoreForExtraLife = baseScoreForExtraLife;
    multiplier = 1;
    multiplierTime = maxMultiplierTime;
    lives = maxLives - 2;
}


void PlayerStatus::addPoints(const int basePoints)
{
    if (isDead())
        return;

    score += basePoints * multiplier;

    while (score >= scoreForExtraLife) {
        scoreForExtraLife += baseScoreForExtraLife;

        if (lives < maxLives)
            lives += 1;
    }
}


void PlayerStatus::increaseMultiplier()
{
    if (isDead())
        return;

    multiplierTime = maxMultiplierTime;

    if (multiplier < maxMultiplier)
        multiplier += 1;
}


void PlayerStatus::removeLife()
{
    lives -= 1;
    multiplier = 1;
    multiplierTime = maxMultiplierTime;
    needBaseReset = true;

    if (isGameOver())
    {
        stopRoundClock();
        respawnTime = 5.f;
    }
    else
    {
        respawnTime = 3.f;
    }

    // Add particles for a grand player explosion!
    float hue1 = ColorPicker::instance().generateHue();
    float hue2 = ColorPicker::instance().generateShiftedHue(hue1);
    sf::Color color1 = ColorPicker::instance().hsvToRgb(hue1, 0.7f, 1.f);
    sf::Color color2 = ColorPicker::instance().hsvToRgb(hue2, 0.7f, 1.f);
    std::uniform_real_distribution<float> particleStartOffset {0.f, PI / killParticleCount};
    std::uniform_real_distribution<float> magnitude {2.f, 24.f};
    const float startOffset = particleStartOffset(randEngine);

    for (int i = 0; i < killParticleCount; i++)
    {
        const sf::Vector2f sprayVelocity = Extensions::fromPolar(PI * 2 * i / killParticleCount + startOffset, magnitude(instance().randEngine));
        const sf::Vector2f position = PlayerShip::instance().getPosition() + 2.f * sprayVelocity;
        Particles::instance().create(
            3.f,
            DontIgnoreGravity,
            Massive,
            position,
            sprayVelocity,
            ColorPicker::instance().lerp(color1, color2)
        );

        // Every particles pick new colors
        if (i % 16 == 0)
        {
            hue1 = ColorPicker::instance().generateHue();
            hue2 = ColorPicker::instance().generateShiftedHue(hue1);
            color1 = ColorPicker::instance().hsvToRgb(hue1, 0.7f, 1.f);
            color2 = ColorPicker::instance().hsvToRgb(hue2, 0.7f, 1.f);
        }
    }

    // Grid explosions
    Grid::instance().applyExplosiveForce(PlayerShip::instance().getPosition(), 500.f, 200.f, 0.3f);
}


void PlayerStatus::loadHighscore()
{
    // Load the high score file and make sure it opened
    std::ifstream highscoreFile {"Content\\Data\\highscores.txt"};

    // If the file is not found just default highscore
    if (!highscoreFile) {
        highScore = 25'000;
        return;
    }

    // The highscore is the only thing in the file
    highscoreFile >> highScore;

    highscoreFile.close();
}


void PlayerStatus::saveHighscore(const int newHighscore)
{
    std::ofstream highscoreFile {"Content\\Data\\highscores.txt"};

    // Don't do anything if the highscore file is not open
    if (!highscoreFile)
        return;

    // Write out the new high score
    highscoreFile << newHighscore;

    highscoreFile.close();
}


bool PlayerStatus::isGameOver() const
{
    return lives == 0;
}


void PlayerStatus::markForKill()
{
    if (!shouldKill && !isDead())
        shouldKill = true;
}


void PlayerStatus::kill()
{
    removeLife();
    shouldKill = false;
}


bool PlayerStatus::isDead() const
{
    return respawnTime > 0.f;
}


void PlayerStatus::update()
{
    // Update the round time seconds
    roundTimeSeconds = roundClock.getElapsedTime().asSeconds();

    if (shouldKill)
        kill();

    // Make sure the player is alive
    if (isDead())
    {
        respawnTime -= GameRoot::instance().deltaTime;

        // When the game was over and the long respawn ends
        if (respawnTime <= 0 && isGameOver())
        {
            needTotalReset = true;
            roundClock.restart();
        }

        return;
    }

    // Multiplier updates
    if (multiplier > 1)
    {
        multiplierTime -= GameRoot::instance().deltaTime;

        if (multiplierTime <= 0)
        {
            multiplierTime = maxMultiplierTime;
            multiplier = 1;
        }
    }
}