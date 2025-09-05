#ifndef PLAYERSTATUS_H
#define PLAYERSTATUS_H


class PlayerStatus {
private:
    const float multiplierMaxTime = 0.8;
    const int maxMultiplier = 20;
    const int baseScoreForExtraLife = 10'000;
    const int maxLives = 5;
    int scoreForExtraLife = baseScoreForExtraLife;
    float multiplierTimeLeft = 0;
    float timeUntilRespawn = 0.0f;
    bool shouldKill = false;

    void kill();
    void loadHighscore();
    void saveHighscore(int) const;

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
