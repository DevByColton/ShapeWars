#ifndef SHAPEKEEPERHEALTHBAR_H
#define SHAPEKEEPERHEALTHBAR_H
#include "SFML/Graphics/Sprite.hpp"


struct ShapeKeeperHealthBar final: sf::Sprite
{
    explicit ShapeKeeperHealthBar(const sf::Texture& texture, const sf::Vector2f &position);

    sf::Sprite background;

    void update(int currentHealth, int maxHealth);
};



#endif //SHAPEKEEPERHEALTHBAR_H
