#ifndef GAMEPLAYHUD_H
#define GAMEPLAYHUD_H
#include "../../../Content/Include/Art.h"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Text.hpp"


struct GamePlayHUD {
    GamePlayHUD();

    sf::Sprite scoreFrame {Art::instance().scoreFrame};
    sf::Text scoreText {Art::instance().font, {}, 60};
    sf::Text multiplierHeaderText {Art::instance().font, {"mult x "}, 20};
    sf::Text multiplierText {Art::instance().font, {}, 20};
    sf::Text timerText {Art::instance().font, {}, 20};
    sf::Text gameOverText {Art::instance().font, {}, 60};

    static std::string formattedTime();
    void drawToScreen();
};



#endif //GAMEPLAYHUD_H
