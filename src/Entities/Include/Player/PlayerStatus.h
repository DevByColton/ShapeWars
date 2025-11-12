#ifndef PLAYERSTATUS_H
#define PLAYERSTATUS_H
#include "SFML/System/Clock.hpp"


class PlayerStatus {
private:
    static constexpr int KILL_PARTICLE_COUNT = 1500;
    static constexpr int MAX_MULTIPLIER = 20;
    static constexpr int MAX_LIVES = 5;
    static constexpr float MAX_MULTIPLIER_TIME = 0.8f;
    float respawnTime = 0.f;
    float multiplierTime = MAX_MULTIPLIER_TIME;
    bool shouldKill = false;
    sf::Clock roundClock {};

    void kill();
    void loadHighscore();
    static void saveHighscore(int);

public:
    PlayerStatus();

    static PlayerStatus &instance() {
        static auto *instance = new PlayerStatus;
        return *instance;
    }

    int highScore = 10'000;
    int score = 0;
    int lives = MAX_LIVES;
    int multiplier = 1;
    bool needTotalReset = false;
    bool needBaseReset = false;
    float roundTimeSeconds = 0.f;

    void stopRoundClock();
    void startRoundClock();
    void markForKill();
    void reset();
    bool isDead() const;
    void update();
    void addPoints(int);
    void removeLife(bool removeAll);
    void increaseMultiplier();
    bool isGameOver() const;
};



#endif //PLAYERSTATUS_H
