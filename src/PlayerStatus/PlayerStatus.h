#ifndef PLAYERSTATUS_H
#define PLAYERSTATUS_H
#include <random>
#include "SFML/System/Clock.hpp"


class PlayerStatus {
private:
    const int killParticleCount = 1400;
    const int maxMultiplier = 20;
    const int baseScoreForExtraLife = 10'000;
    const int maxLives = 5;
    const float maxMultiplierTime = 2.f;
    sf::Clock roundClock {};
    float respawnTime = 0.f;
    float multiplierTime = maxMultiplierTime;
    int scoreForExtraLife = baseScoreForExtraLife;
    bool shouldKill = false;

    void kill();
    void loadHighscore();
    static void saveHighscore(int);

public:
    PlayerStatus();
    ~PlayerStatus() = default;

    static PlayerStatus &instance() {
        static auto *instance = new PlayerStatus;
        return *instance;
    }

    int highScore = -1;
    int score = 0;
    int lives = 4;
    int multiplier = 1;
    bool needTotalReset = false;
    bool needBaseReset = false;
    float roundTimeSeconds = 0.f;
    std::default_random_engine randEngine {std::random_device{}()};

    void stopRoundClock();
    void startRoundClock();
    void markForKill();
    void reset();
    bool isDead() const;
    void update();
    void addPoints(int);
    void removeLife();
    void increaseMultiplier();
    bool isGameOver() const;
};



#endif //PLAYERSTATUS_H
