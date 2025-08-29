#include "Art.h"
#include "../Logger/Logger.h"
#include "SFML/System/Exception.hpp"

Art::Art() {
    try {
        Player = sf::Texture("Content\\Images\\Player.png");
        Seeker = sf::Texture("Content\\Images\\Seeker.png");
        Wanderer = sf::Texture("Content\\Images\\Wanderer.png");
        Bullet = sf::Texture("Content\\Images\\Bullet.png");
        Pointer = sf::Texture("Content\\Images\\Pointer.png");
    } catch (sf::Exception &ex) {
        Logger::printError(ex.what());
    }
}
