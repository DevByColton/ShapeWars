#ifndef PLAYERSTATUS_H
#define PLAYERSTATUS_H
#include <random>
#include "SFML/System/Clock.hpp"


class PlayerStatus {
private:
    const int KILL_PARTICLE_COUNT = 1400;
    const int MAX_MULTIPLIER = 20;
    const int MAX_LIVES = 5;
    const float MAX_MULTIPLIER_TIME = 2.f;
    float respawnTime = 0.f;
    float multiplierTime = MAX_MULTIPLIER_TIME;
    bool shouldKill = false;
    sf::Clock roundClock {};

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
    int lives = MAX_LIVES;
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
