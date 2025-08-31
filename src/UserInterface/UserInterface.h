#ifndef USERINTERFACE_H
#define USERINTERFACE_H
#include "../Content/Art.h"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Text.hpp"

class UserInterface {
private:
    UserInterface();
    ~UserInterface() = default;

    sf::Sprite playerShipSprite {Art::instance().player};
    sf::Text pausedText {Art::instance().font, {"== [ paused ] =="}, 72};
    sf::Text livesText {Art::instance().font, {"== [ lives ] =="}, 30};
    sf::Text scoreText {Art::instance().font, {}, 60};
    sf::Text multiplierHeaderText {Art::instance().font, {"mult x "}, 20};
    sf::Text multiplierText {Art::instance().font, {}, 20};
    sf::Text highScoreHeaderText {Art::instance().font, {"== [ highscore ] =="}, 30};
    sf::Text highScoreText {Art::instance().font, {}, 30};
    sf::Text gameOverText {Art::instance().font, {}, 60};

    float playerShipWidthScaled() const;

public:
    static UserInterface &instance() {
        static auto *instance = new UserInterface;
        return *instance;
    }

    void draw();
};



#endif //USERINTERFACE_H
