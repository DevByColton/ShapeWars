#include "../../Include/ShapeKeeper/ShapeKeeperHealthBar.h"
#include "SFML/Graphics/Texture.hpp"


ShapeKeeperHealthBar::ShapeKeeperHealthBar(const sf::Texture& texture, const sf::Vector2f &position) : Sprite(texture), background(texture)
{
    setPosition(position);
    setOrigin({getTexture().getSize().x / 2.f, getTexture().getSize().y / 2.f});
    background.setPosition(getPosition());
    background.setOrigin(getOrigin());
    background.setColor({getColor().r, getColor().g, getColor().b, 64});
}


void ShapeKeeperHealthBar::update(const int currentHealth, const int maxHealth)
{
    sf::Vector2i size {0, static_cast<int>(getTexture().getSize().y)};

    if (currentHealth > 0)
    {
        // Normalize the health to the sprite width, or full width if health is full
        int x = static_cast<int>(getTexture().getSize().x);
        if (currentHealth != maxHealth)
            x = currentHealth * getTexture().getSize().x / maxHealth;

        // Set the health bar to reflect remaining health.
        // If the health is less 3 then do not reduce the width anymore,
        // It is basically not visible under 3 pixels so the body part could appear to be dead when it only
        // has a couple hit points left
        size.x = x < 3 ? 3 : x;
    }

    setTextureRect({{0,0}, size});
}
