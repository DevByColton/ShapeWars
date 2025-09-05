#ifndef ART_H
#define ART_H
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/Texture.hpp"


struct Art {
    Art();
    ~Art() = default;

    static Art &instance() {
        static auto *instance = new Art;
        return *instance;
    }

    sf::Texture player;
    sf::Texture seeker;
    sf::Texture wanderer;
    sf::Texture bullet;
    sf::Texture pointer;
    sf::Texture enemyPlaceholder;
    sf::Texture blackHole;
    sf::Font font;
};



#endif //ART_H
