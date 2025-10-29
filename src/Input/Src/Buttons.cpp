#include "../Include/Buttons.h"
#include "../Include/Input.h"


Buttons::Buttons()
{
    // Xbox buttons
    xboxAButton.setOrigin({
        xboxAButton.getTexture().getSize().x / 2.f,
        xboxAButton.getTexture().getSize().y / 2.f
    });

    xboxBButton.setOrigin({
        xboxBButton.getTexture().getSize().x / 2.f,
        xboxBButton.getTexture().getSize().y / 2.f
    });

    xboxXButton.setOrigin({
        xboxXButton.getTexture().getSize().x / 2.f,
        xboxXButton.getTexture().getSize().y / 2.f
    });

    xboxYButton.setOrigin({
        xboxYButton.getTexture().getSize().x / 2.f,
        xboxYButton.getTexture().getSize().y / 2.f
    });

    xboxRightTrigger.setOrigin({
        xboxRightTrigger.getTexture().getSize().x / 2.f,
        xboxRightTrigger.getTexture().getSize().y / 2.f
    });

    // Dualsense buttons
    dualsenseXButton.setOrigin({
        dualsenseXButton.getTexture().getSize().x / 2.f,
        dualsenseXButton.getTexture().getSize().y / 2.f
    });

    dualsenseCircleButton.setOrigin({
        dualsenseCircleButton.getTexture().getSize().x / 2.f,
        dualsenseCircleButton.getTexture().getSize().y / 2.f
    });

    dualsenseSquareButton.setOrigin({
        dualsenseSquareButton.getTexture().getSize().x / 2.f,
        dualsenseSquareButton.getTexture().getSize().y / 2.f
    });

    dualsenseTriangleButton.setOrigin({
        dualsenseTriangleButton.getTexture().getSize().x / 2.f,
        dualsenseTriangleButton.getTexture().getSize().y / 2.f
    });

    dualsenseRightTrigger.setOrigin({
        dualsenseRightTrigger.getTexture().getSize().x / 2.f,
        dualsenseRightTrigger.getTexture().getSize().y / 2.f
    });
}


Buttons::Key::Key(const sf::Texture& texture) : Sprite(texture)
{
    setOrigin({
        getTexture().getSize().x / 2.f,
        getTexture().getSize().y / 2.f
    });

    frame.setOrigin({
        frame.getTexture().getSize().x / 2.f,
        frame.getTexture().getSize().y / 2.f
    });
}


void Buttons::Key::setFramePosition(const sf::Vector2f& position)
{
    frame.setPosition(position);
    setPosition(position);
}


void Buttons::draw(sf::RenderTexture& renderTexture) const
{
    // Favor xbox buttons if there is a valid xbox input
    if (Input::instance().hasValidXboxInput)
    {
        renderTexture.draw(xboxAButton);
        renderTexture.draw(xboxBButton);
        renderTexture.draw(xboxXButton);
        renderTexture.draw(xboxYButton);
        renderTexture.draw(xboxRightTrigger);
        return;
    }

    // Dualsense
    if (Input::instance().hasValidDualsenseInput)
    {
        renderTexture.draw(dualsenseXButton);
        renderTexture.draw(dualsenseCircleButton);
        renderTexture.draw(dualsenseSquareButton);
        renderTexture.draw(dualsenseTriangleButton);
        renderTexture.draw(dualsenseRightTrigger);
        return;
    }

    // Always draw keys as a fallthrough
    renderTexture.draw(num1.frame);
    renderTexture.draw(num1);

    renderTexture.draw(num2.frame);
    renderTexture.draw(num2);

    renderTexture.draw(num3.frame);
    renderTexture.draw(num3);

    renderTexture.draw(num4.frame);
    renderTexture.draw(num4);

    renderTexture.draw(spaceBar.frame);
    renderTexture.draw(spaceBar);
}
