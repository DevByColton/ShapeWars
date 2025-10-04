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
    sf::Texture nuke;
    sf::Texture seeker;
    sf::Texture wanderer;
    sf::Texture dodger;
    sf::Texture snakeHead;
    sf::Texture snakeBody;
    sf::Texture snakeTail;
    sf::Texture bullet;
    sf::Texture pointer;
    sf::Texture enemyPlaceholder;
    sf::Texture blackHole;
    sf::Texture particle;
    sf::Texture particleGlow;
    sf::Texture buffFrame;
    sf::Texture buffIconPlaceHolder;
    sf::Texture bulletsAllDirectionsBuff;
    sf::Texture invincibleBuff;
    sf::Texture shotGunBuff;
    sf::Texture boostersBuff;
    sf::Texture livesAndNukesFrame;
    sf::Texture scoreFrame;
    sf::Texture keyboardFrame;
    sf::Texture num1;
    sf::Texture num2;
    sf::Texture num3;
    sf::Texture num4;
    sf::Texture spaceBar;
    sf::Texture dualsenseXButton;
    sf::Texture dualsenseSquareButton;
    sf::Texture dualsenseCircleButton;
    sf::Texture dualsenseTriangleButton;
    sf::Texture dualsenseRightTrigger;
    sf::Texture xboxAButton;
    sf::Texture xboxBButton;
    sf::Texture xboxXButton;
    sf::Texture xboxYButton;
    sf::Texture xboxRightTrigger;
    sf::Font font;
};



#endif //ART_H
