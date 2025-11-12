#include <fstream>
#include "../../Include/Player/PlayerStatus.h"
#include "../../../GameRoot.h"
#include "../../../Systems/Include/Grid.h"
#include "../../../Systems/Include/Particles.h"
#include "../../../Core/Include/ColorPicker.h"
#include "../../../Core/Include/RandomVector.h"
#include "../../../GameState/Include/GamePlay.h"
#include "../../Include/BlackHoles.h"
#include "../../Include/Enemies.h"
#include "../../Include/Player/PlayerShip.h"


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
    multiplier = 1;
    multiplierTime = MAX_MULTIPLIER_TIME;
    lives = MAX_LIVES;
}


void PlayerStatus::addPoints(const int basePoints)
{
    if (isDead())
        return;

    score += basePoints * multiplier;
}


void PlayerStatus::increaseMultiplier()
{
    if (isDead())
        return;

    multiplierTime = MAX_MULTIPLIER_TIME;

    if (multiplier < MAX_MULTIPLIER)
        multiplier += 1;
}


void PlayerStatus::removeLife(const bool removeAll)
{
    lives -= removeAll ? lives : 1;
    multiplier = 1;
    multiplierTime = MAX_MULTIPLIER_TIME;
    needBaseReset = true;

    if (isGameOver())
    {
        stopRoundClock();
        respawnTime = 5.f;
        GamePlay::instance().endRound();
    }
    else
    {
        respawnTime = 2.f;
    }

    // Grid explosion
    Grid::instance().applyExplosiveForce(PlayerShip::instance().getPosition(), 500.f, 200.f, 0.3f);

    // Add particles for a grand player explosion!
    float hue1 = ColorPicker::instance().generateHue();
    float hue2 = ColorPicker::instance().generateShiftedHue(hue1);
    sf::Color color1 = ColorPicker::instance().hsvToRgb(hue1, 0.9f, 0.8f);
    sf::Color color2 = ColorPicker::instance().hsvToRgb(hue2, 0.9f, 0.8f);

    for (int i = 0; i < KILL_PARTICLE_COUNT; i++)
    {
        Particles::instance().create(
            1.5f,
            DontIgnoreGravity,
            Explosion,
            PlayerShip::instance().getPosition(),
            RandomVector::instance().next(2.f, 48.f),
            ColorPicker::instance().lerp(color1, color2)
        );

        // Periodically pick new colors
        if (i % 16 == 0)
        {
            hue1 = ColorPicker::instance().generateHue();
            hue2 = ColorPicker::instance().generateShiftedHue(hue1);
            color1 = ColorPicker::instance().hsvToRgb(hue1, 0.9f, 0.8f);
            color2 = ColorPicker::instance().hsvToRgb(hue2, 0.9f, 0.8f);
        }
    }
}


void PlayerStatus::loadHighscore()
{
    // Open the highscore file
    std::fstream highscoreFile {"Content\\Data\\Highscore.bin", std::ios::in | std::ios::binary};

    // If the file is not found just default highscore
    if (!highscoreFile)
        return;

    // Read the options and close
    highscoreFile.read(reinterpret_cast<char*>(&highScore), sizeof(highScore));
    highscoreFile.close();
}


void PlayerStatus::saveHighscore(int newHighscore)
{
    // Open the options file
    std::fstream highscoreFile {"Content\\Data\\Highscore.bin", std::ios::out | std::ios::binary};

    if (!highscoreFile)
        return;

    // Read the options and close
    highscoreFile.write(reinterpret_cast<char*>(&newHighscore), sizeof(newHighscore));
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
    removeLife(false);
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
    {
        kill();
        Enemies::instance().canSpawn = false;
        BlackHoles::instance().canSpawn = false;
    }

    // Make sure the player is alive
    if (isDead())
    {
        respawnTime -= GameRoot::instance().deltaTime;

        if (respawnTime <= 0)
        {
            Enemies::instance().canSpawn = true;
            BlackHoles::instance().canSpawn = true;

            // When the game was over and the long respawn ends
            if (isGameOver())
            {
                needTotalReset = true;
                roundClock.restart();
            }
        }

        return;
    }

    // Multiplier updates
    if (multiplier > 1)
    {
        multiplierTime -= GameRoot::instance().deltaTime;

        if (multiplierTime <= 0)
        {
            multiplierTime = MAX_MULTIPLIER_TIME;
            multiplier = 1;
        }
    }
}