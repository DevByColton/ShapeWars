#include <fstream>
#include "PlayerStatus.h"
#include "../GameRoot.h"
#include "../Entities/Include/PlayerShip.h"


PlayerStatus::PlayerStatus() {
    loadHighscore();
}


void PlayerStatus::setDefaultHighscore() {
    highScore = 50'000;
}


void PlayerStatus::reset() {

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


void PlayerStatus::update() {

    if (multiplier > 1) {

        multiplierTimeLeft -= GameRoot::instance().deltaTime;

        if (multiplierTimeLeft <= 0) {
            multiplierTimeLeft = multiplierMaxTime;
            multiplier = 1;
        }
    }

}


void PlayerStatus::addPoints(const int basePoints) {

    if (PlayerShip::instance().isDead())
        return;

    score += basePoints * multiplier;

    while (score >= scoreForExtraLife) {
        scoreForExtraLife += baseScoreForExtraLife;

        if (lives < maxLives)
            lives += 1;
    }

}

void PlayerStatus::increaseMultiplier() {

    if (PlayerShip::instance().isDead())
        return;

    multiplierTimeLeft = multiplierMaxTime;

    if (multiplier < maxMultiplier)
        multiplier += 1;

}


void PlayerStatus::removeLife() {
    lives -= 1;
}


void PlayerStatus::loadHighscore() {

    // Load the high score file and make sure it opened
    std::ifstream highscoreFile {"Content\\Data\\highscores.txt"};

    // If the file is not found just default highscore
    if (!highscoreFile) {
        setDefaultHighscore();
        return;
    }

    // The highscore is the only thing in the file
    highscoreFile >> highScore;

    highscoreFile.close();
}


void PlayerStatus::saveHighscore(const int newHighscore) const {

    std::ofstream highscoreFile {"Content\\Data\\highscores.txt"};

    // Don't do anything if the highscore file is not open
    if (!highscoreFile)
        return;

    // Write out the new high score
    highscoreFile << newHighscore;

    highscoreFile.close();
}


bool PlayerStatus::isGameOver() const {
    return lives == 0;
}
