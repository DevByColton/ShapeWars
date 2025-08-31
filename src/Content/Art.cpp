#include "Art.h"
#include "../Logger/Logger.h"
#include "SFML/System/Exception.hpp"

Art::Art() {

    try {
        player = sf::Texture("Content\\Images\\Player.png");
        seeker = sf::Texture("Content\\Images\\Seeker.png");
        wanderer = sf::Texture("Content\\Images\\Wanderer.png");
        bullet = sf::Texture("Content\\Images\\Bullet.png");
        pointer = sf::Texture("Content\\Images\\Pointer.png");
        enemyPlaceholder = sf::Texture("Content\\Images\\EnemyPlaceholder.png");
        font = sf::Font("Content\\Font\\TurretRoad-Regular.ttf");
    } catch (sf::Exception &ex) {
        Logger::printError(ex.what());
    }

}
