#include <fstream>
#include "PlayerStatus.h"
#include "../GameRoot.h"


PlayerStatus::PlayerStatus()
{
    loadHighscore();
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
    lives = 4;
    multiplierTimeLeft = 0;
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

    multiplierTimeLeft = multiplierMaxTime;

    if (multiplier < maxMultiplier)
        multiplier += 1;
}


void PlayerStatus::removeLife()
{
    lives -= 1;
    needBaseReset = true;

    if (isGameOver())
    {
        timeUntilRespawn = 5.0;
        GameRoot::instance().stopTotalGameClock();
    }
    else
    {
        timeUntilRespawn = 1.0;
    }
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


void PlayerStatus::saveHighscore(const int newHighscore) const
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
    // Only set should kill to true if it was not already marked this frame
    if (!shouldKill)
        shouldKill = true;
}


void PlayerStatus::kill()
{
    removeLife();
    shouldKill = false;
}


bool PlayerStatus::isDead() const
{
    return timeUntilRespawn > 0.0;
}


void PlayerStatus::update()
{
    if (shouldKill)
        kill();

    // Make sure the player is alive
    if (isDead()) {
        timeUntilRespawn -= GameRoot::instance().deltaTime;

        // When the game was over and the long respawn ends
        if (timeUntilRespawn <= 0 && isGameOver())
            needTotalReset = true;

        return;
    }

    // Multiplier updates
    if (multiplier > 1)
    {
        multiplierTimeLeft -= GameRoot::instance().deltaTime;

        if (multiplierTimeLeft <= 0)
        {
            multiplierTimeLeft = multiplierMaxTime;
            multiplier = 1;
        }
    }
}