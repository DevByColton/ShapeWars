#include "../Include/Buttons.h"
#include "../../GameRoot.h"
#include "../Include/Input.h"


Buttons::Buttons()
{
    // Set keyboard frame 2 origin and position
    num2.frame.setOrigin({
        num2.frame.getTexture().getSize().x / 2.f,
        num2.frame.getTexture().getSize().y / 2.f
    });
    num2.frame.setPosition({
        GameRoot::instance().windowSizeF.x / 2.f - 40.f,
        GameRoot::instance().windowSizeF.y - 22.5f
    });
    num2.key.setTexture(Art::instance().num2);
    num2.key.setOrigin({
        num2.key.getTexture().getSize().x / 2.f,
        num2.key.getTexture().getSize().y / 2.f
    });
    num2.key.setPosition(num2.frame.getPosition());

    // Set keyboard frame 1 origin and position
    num1.frame.setOrigin({
        num1.frame.getTexture().getSize().x / 2.f,
        num1.frame.getTexture().getSize().y / 2.f
    });
    num1.frame.setPosition({
        num2.frame.getPosition().x - 80.f,
        GameRoot::instance().windowSizeF.y - 22.5f
    });
    num1.key.setTexture(Art::instance().num1);
    num1.key.setOrigin({
        num1.key.getTexture().getSize().x / 2.f,
        num1.key.getTexture().getSize().y / 2.f
    });
    num1.key.setPosition(num1.frame.getPosition());

    // Set keyboard frame 3 origin and position
    num3.frame.setOrigin({
        num3.frame.getTexture().getSize().x / 2.f,
        num3.frame.getTexture().getSize().y / 2.f
    });
    num3.frame.setPosition({
        GameRoot::instance().windowSizeF.x / 2.f + 40.f,
        GameRoot::instance().windowSizeF.y - 22.5f
    });
    num3.key.setTexture(Art::instance().num3);
    num3.key.setOrigin({
        num3.key.getTexture().getSize().x / 2.f,
        num3.key.getTexture().getSize().y / 2.f
    });
    num3.key.setPosition(num3.frame.getPosition());

    // Set keyboard frame 4 origin and position
    num4.frame.setOrigin({
        num4.frame.getTexture().getSize().x / 2.f,
        num4.frame.getTexture().getSize().y / 2.f
    });
    num4.frame.setPosition({
        num3.frame.getPosition().x + 80.f,
        GameRoot::instance().windowSizeF.y - 22.5f
    });
    num4.key.setTexture(Art::instance().num4);
    num4.key.setOrigin({
        num4.key.getTexture().getSize().x / 2.f,
        num4.key.getTexture().getSize().y / 2.f
    });
    num4.key.setPosition(num4.frame.getPosition());

    // Set the space bar origin and position
    spaceBar.frame.setOrigin({
        spaceBar.frame.getTexture().getSize().x / 2.f,
        spaceBar.frame.getTexture().getSize().y / 2.f
    });
    spaceBar.frame.setPosition({
        num4.frame.getPosition().x + 60.f,
        GameRoot::instance().windowSizeF.y - 22.5f
    });
    spaceBar.key.setTexture(Art::instance().spaceBar);
    spaceBar.key.setOrigin({
        spaceBar.key.getTexture().getSize().x / 2.f,
        spaceBar.key.getTexture().getSize().y / 2.f
    });
    spaceBar.key.setPosition(spaceBar.frame.getPosition());

    // Xbox buttons
    xboxAButton.setOrigin({
        xboxAButton.getTexture().getSize().x / 2.f,
        xboxAButton.getTexture().getSize().y / 2.f
    });
    xboxAButton.setPosition(num1.key.getPosition());

    xboxBButton.setOrigin({
        xboxBButton.getTexture().getSize().x / 2.f,
        xboxBButton.getTexture().getSize().y / 2.f
    });
    xboxBButton.setPosition(num2.key.getPosition());

    xboxXButton.setOrigin({
        xboxXButton.getTexture().getSize().x / 2.f,
        xboxXButton.getTexture().getSize().y / 2.f
    });
    xboxXButton.setPosition(num3.key.getPosition());

    xboxYButton.setOrigin({
        xboxYButton.getTexture().getSize().x / 2.f,
        xboxYButton.getTexture().getSize().y / 2.f
    });
    xboxYButton.setPosition(num4.key.getPosition());

    xboxRightTrigger.setOrigin({
        xboxRightTrigger.getTexture().getSize().x / 2.f,
        xboxRightTrigger.getTexture().getSize().y / 2.f
    });
    xboxRightTrigger.setPosition(spaceBar.key.getPosition());

    // Dualsense buttons
    dualsenseXButton.setOrigin({
        dualsenseXButton.getTexture().getSize().x / 2.f,
        dualsenseXButton.getTexture().getSize().y / 2.f
    });
    dualsenseXButton.setPosition(num1.key.getPosition());

    dualsenseCircleButton.setOrigin({
        dualsenseCircleButton.getTexture().getSize().x / 2.f,
        dualsenseCircleButton.getTexture().getSize().y / 2.f
    });
    dualsenseCircleButton.setPosition(num2.key.getPosition());

    dualsenseSquareButton.setOrigin({
        dualsenseSquareButton.getTexture().getSize().x / 2.f,
        dualsenseSquareButton.getTexture().getSize().y / 2.f
    });
    dualsenseSquareButton.setPosition(num3.key.getPosition());

    dualsenseTriangleButton.setOrigin({
        dualsenseTriangleButton.getTexture().getSize().x / 2.f,
        dualsenseTriangleButton.getTexture().getSize().y / 2.f
    });
    dualsenseTriangleButton.setPosition(num4.key.getPosition());

    dualsenseRightTrigger.setOrigin({
        dualsenseRightTrigger.getTexture().getSize().x / 2.f,
        dualsenseRightTrigger.getTexture().getSize().y / 2.f
    });
    dualsenseRightTrigger.setPosition(spaceBar.key.getPosition());
}


void Buttons::draw()
{
    // Favor xbox buttons if there is a valid xbox input
    if (Input::instance().hasValidXboxInput)
    {
        GameRoot::instance().renderWindow.draw(xboxAButton);
        GameRoot::instance().renderWindow.draw(xboxBButton);
        GameRoot::instance().renderWindow.draw(xboxXButton);
        GameRoot::instance().renderWindow.draw(xboxYButton);
        GameRoot::instance().renderWindow.draw(xboxRightTrigger);
        return;
    }

    // Dualsense
    if (Input::instance().hasValidDualsenseInput)
    {
        GameRoot::instance().renderWindow.draw(dualsenseXButton);
        GameRoot::instance().renderWindow.draw(dualsenseCircleButton);
        GameRoot::instance().renderWindow.draw(dualsenseSquareButton);
        GameRoot::instance().renderWindow.draw(dualsenseTriangleButton);
        GameRoot::instance().renderWindow.draw(dualsenseRightTrigger);
        return;
    }

    // Always draw keys as a fallthrough
    GameRoot::instance().renderWindow.draw(num1.frame);
    GameRoot::instance().renderWindow.draw(num1.key);

    GameRoot::instance().renderWindow.draw(num2.frame);
    GameRoot::instance().renderWindow.draw(num2.key);

    GameRoot::instance().renderWindow.draw(num3.frame);
    GameRoot::instance().renderWindow.draw(num3.key);

    GameRoot::instance().renderWindow.draw(num4.frame);
    GameRoot::instance().renderWindow.draw(num4.key);

    GameRoot::instance().renderWindow.draw(spaceBar.frame);
    GameRoot::instance().renderWindow.draw(spaceBar.key);
}
