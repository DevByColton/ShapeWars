#ifndef NUKES_H
#define NUKES_H
#include "SFML/Graphics/CircleShape.hpp"


class Nukes
{
private:
    const int MAX_NUKE_COUNT = 5;
    const float INITIAL_RADIUS = 100.f;
    const float INITIAL_VELOCITY = 25.f;
    float velocity = INITIAL_VELOCITY;
    sf::Color nukeColor {255, 211, 38, 255};
    sf::CircleShape nukeCircle {radius, 100};

public:
    Nukes();

    static Nukes &instance()
    {
        static auto *instance = new Nukes;
        return *instance;
    }

    static constexpr float TIME_UNTIL_ENEMIES_SPAWN_AFTER_DETONATION_DURATION = 3.5f;
    float timeUntilEnemiesSpawnAfterElapsed = 0.f;
    bool isDetonating = false;
    int count = MAX_NUKE_COUNT;
    float radius = INITIAL_RADIUS;

    void reset();
    void resetEnemiesSpawnTimer();
    void resetNukeCount();
    void markDetonate(const sf::Vector2f &fromPosition);
    void startEnemiesSpawnTimer();
    void updateEnemiesSpawnTimer();
    void update();
    void draw() const;
    sf::Vector2f getPosition() const;
};



#endif //NUKES_H
