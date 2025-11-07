#ifndef BUTTONS_H
#define BUTTONS_H
#include "Input.h"
#include "../../Content/Include/Art.h"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/RenderTexture.hpp"


struct Buttons {

    struct Button final : sf::Sprite
    {
        explicit Button(const sf::Texture& texture, ButtonsOverride type);

        ButtonsOverride type;
    };

    Button num1 {Art::instance().num1, ButtonsOverride::Keyboard};
    Button num2 {Art::instance().num2, ButtonsOverride::Keyboard};
    Button num3 {Art::instance().num3, ButtonsOverride::Keyboard};
    Button num4 {Art::instance().num4, ButtonsOverride::Keyboard};
    Button spaceBar {Art::instance().spaceBar, ButtonsOverride::Keyboard};
    Button escape {Art::instance().escape, ButtonsOverride::Keyboard};

    Button xboxAButton {Art::instance().xboxAButton, ButtonsOverride::Xbox};
    Button xboxBButton {Art::instance().xboxBButton, ButtonsOverride::Xbox};
    Button xboxXButton {Art::instance().xboxXButton, ButtonsOverride::Xbox};
    Button xboxYButton {Art::instance().xboxYButton, ButtonsOverride::Xbox};
    Button xboxRightTrigger {Art::instance().xboxRightTrigger, ButtonsOverride::Xbox};

    Button dualsenseXButton {Art::instance().dualsenseXButton, ButtonsOverride::Dualsense};
    Button dualsenseCircleButton {Art::instance().dualsenseCircleButton, ButtonsOverride::Dualsense};
    Button dualsenseSquareButton {Art::instance().dualsenseSquareButton, ButtonsOverride::Dualsense};
    Button dualsenseTriangleButton {Art::instance().dualsenseTriangleButton, ButtonsOverride::Dualsense};
    Button dualsenseRightTrigger {Art::instance().dualsenseRightTrigger, ButtonsOverride::Dualsense};

    std::vector<Button*> activeButtons {nullptr};

    void drawXbox(sf::RenderTexture& renderTexture) const;
    void drawDualsense(sf::RenderTexture& renderTexture) const;
    void drawKeyboard(sf::RenderTexture& renderTexture) const;
    void draw(sf::RenderTexture& renderTexture) const;
};



#endif //BUTTONS_H
