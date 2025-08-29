#ifndef ART_H
#define ART_H
#include "SFML/Graphics/Texture.hpp"


struct Art {
    Art();
    ~Art() = default;

    static Art &instance() {
        static auto *instance = new Art;
        return *instance;
    }

    sf::Texture Player;
    sf::Texture Seeker;
    sf::Texture Wanderer;
    sf::Texture Bullet;
    sf::Texture Pointer;
};



#endif //ART_H
