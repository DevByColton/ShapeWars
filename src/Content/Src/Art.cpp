#include "../Include/Art.h"
#include "../../Logger/Logger.h"
#include "SFML/System/Exception.hpp"

Art::Art()
{
    try {
        player = sf::Texture("Content\\Images\\Player.png");
        player.setSmooth(true);

        seeker = sf::Texture("Content\\Images\\Seeker.png");
        seeker.setSmooth(true);

        wanderer = sf::Texture("Content\\Images\\Wanderer.png");
        wanderer.setSmooth(true);

        bullet = sf::Texture("Content\\Images\\Bullet.png");
        bullet.setSmooth(true);

        pointer = sf::Texture("Content\\Images\\Pointer.png");
        pointer.setSmooth(true);

        enemyPlaceholder = sf::Texture("Content\\Images\\EnemyPlaceholder.png");
        enemyPlaceholder.setSmooth(true);

        blackHole = sf::Texture("Content\\Images\\Black Hole.png");
        blackHole.setSmooth(true);

        particle = sf::Texture("Content\\Images\\Laser.png");
        particle.setSmooth(true);

        particleGlow = sf::Texture("Content\\Images\\Glow.png");
        particleGlow.setSmooth(true);

        font = sf::Font("Content\\Font\\TurretRoad-Regular.ttf");
        font.setSmooth(true);

    } catch (sf::Exception &ex) {
        Logger::printError(ex.what());
    }
}
