#ifndef BUTTONS_H
#define BUTTONS_H
#include "../../Content/Include/Art.h"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/RenderTexture.hpp"


struct Buttons {
    Buttons();

    struct Key final : sf::Sprite
    {
        explicit Key(const sf::Texture &texture);

        sf::Sprite frame {Art::instance().keyboardFrame};

        void setFramePosition(const sf::Vector2f& position);
    };

    Key num1 {Art::instance().num1};
    Key num2 {Art::instance().num2};
    Key num3 {Art::instance().num3};
    Key num4 {Art::instance().num4};
    Key spaceBar {Art::instance().spaceBar};

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

    void draw(sf::RenderTexture& renderTexture) const;
};



#endif //BUTTONS_H
