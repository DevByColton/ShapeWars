#include "../Include/Art.h"
#include "../../Logger/Logger.h"
#include "SFML/System/Exception.hpp"

Art::Art()
{
    try
    {
        player = sf::Texture("Content\\Images\\Player.png");
        player.setSmooth(true);

        nuke = sf::Texture("Content\\Images\\Nuke.png");
        nuke.setSmooth(true);

        seeker = sf::Texture("Content\\Images\\Seeker.png");
        seeker.setSmooth(true);

        wanderer = sf::Texture("Content\\Images\\Wanderer.png");
        wanderer.setSmooth(true);

        dodger = sf::Texture("Content\\Images\\Dodger.png");
        dodger.setSmooth(true);

        bullet = sf::Texture("Content\\Images\\Bullet.png");
        bullet.setSmooth(true);

        pointer = sf::Texture("Content\\Images\\Cursor.png");
        pointer.setSmooth(true);

        enemyPlaceholder = sf::Texture("Content\\Images\\EnemyPlaceholder.png");
        enemyPlaceholder.setSmooth(true);

        blackHole = sf::Texture("Content\\Images\\BlackHole.png");
        blackHole.setSmooth(true);

        particle = sf::Texture("Content\\Images\\Laser.png");
        particle.setSmooth(true);

        particleGlow = sf::Texture("Content\\Images\\Glow.png");
        particleGlow.setSmooth(true);

        font = sf::Font("Content\\Font\\TurretRoad-Regular.ttf");
        font.setSmooth(true);

    }
    catch (sf::Exception &ex)
    {
        Logger::printError(ex.what());
    }
}
