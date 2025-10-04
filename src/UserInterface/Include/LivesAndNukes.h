#ifndef LIVESANDNUKES_H
#define LIVESANDNUKES_H
#include "../../Content/Include/Art.h"
#include "SFML/Graphics/Sprite.hpp"


class LivesAndNukes {
private:
    sf::Sprite livesSprite {Art::instance().player};
    sf::Sprite nukeSprite {Art::instance().nuke};
    sf::Sprite nukesFrame {Art::instance().livesAndNukesFrame};
    sf::Sprite livesFrame {Art::instance().livesAndNukesFrame};

public:
    LivesAndNukes();

    static LivesAndNukes& instance()
    {
        static auto *instance = new LivesAndNukes;
        return *instance;
    }

    void draw();
};



#endif //LIVESANDNUKES_H
