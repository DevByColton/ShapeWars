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
    sf::Color nukeColor {84, 62, 240, 255};
    sf::CircleShape nukeCircle {radius, 100};

    void reset();

public:
    Nukes();

    static Nukes &instance()
    {
        static auto *instance = new Nukes;
        return *instance;
    }

    int count = 10;
    bool isDetonating = false;
    float radius = initialRadius;

    void resetNukeCount();
    void markDetonate(const sf::Vector2f &fromPosition);
    bool update();
    void draw() const;
    sf::Vector2f getPosition() const;
};



#endif //NUKES_H
