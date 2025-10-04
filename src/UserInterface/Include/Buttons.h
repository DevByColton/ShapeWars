#ifndef BUTTONS_H
#define BUTTONS_H
#include "../../Content/Include/Art.h"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Text.hpp"


class Buttons {
private:
    struct Key
    {
        sf::Sprite frame {Art::instance().keyboardFrame};
        sf::Sprite key {Art::instance().num1}; // Placeholder key
    };

    struct Button
    {
        sf::Sprite frame {Art::instance().keyboardFrame};
        sf::Sprite button {Art::instance().xboxAButton}; // Placeholder button
    };

    Key num1 {};
    Key num2 {};
    Key num3 {};
    Key num4 {};
    Key spaceBar {};

    sf::Sprite xboxAButton {Art::instance().xboxAButton};
    sf::Sprite xboxBButton {Art::instance().xboxBButton};
    sf::Sprite xboxXButton {Art::instance().xboxXButton};
    sf::Sprite xboxYButton {Art::instance().xboxYButton};
    sf::Sprite xboxRightTrigger {Art::instance().xboxRightTrigger};

    sf::Sprite dualsenseXButton {Art::instance().dualsenseXButton};
    sf::Sprite dualsenseCircleButton {Art::instance().dualsenseCircleButton};
    sf::Sprite dualsenseSquareButton {Art::instance().dualsenseSquareButton};
    sf::Sprite dualsenseTriangleButton {Art::instance().dualsenseTriangleButton};
    sf::Sprite dualsenseRightTrigger {Art::instance().dualsenseRightTrigger};

public:
    Buttons();

    static Buttons &instance()
    {
        static auto *instance = new Buttons;
        return *instance;
    }

    void draw();
};



#endif //BUTTONS_H
