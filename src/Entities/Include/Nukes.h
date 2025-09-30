#ifndef NUKES_H
#define NUKES_H
#include "SFML/Graphics/CircleShape.hpp"


class Nukes
{
private:
    const float initialRadius = 100.f;
    const float initialVelocity = 25.f;
    const int max = 5;
    float velocity = initialVelocity;
    sf::Color nukeColor {255, 240, 140, 255};
    sf::CircleShape nukeCircle {radius, 100};

public:
    Nukes();

    static Nukes &instance()
    {
        static auto *instance = new Nukes;
        return *instance;
    }

    int count = 3;
    bool isDetonating = false;
    float radius = initialRadius;

    void reset();
    void resetNukeCount();
    void markDetonate(const sf::Vector2f &fromPosition);
    void update();
    void draw() const;
    sf::Vector2f getPosition() const;
};



#endif //NUKES_H
