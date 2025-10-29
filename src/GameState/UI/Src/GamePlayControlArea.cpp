#include "../Include/GamePlayControlArea.h"
#include "../../../GameRoot.h"
#include "../../../Content/Include/GaussianBlur.h"
#include "../../../Core/Include/Extensions.h"
#include "../../../Entities/Include/Nukes.h"
#include "../../../Entities/Include/Player/Buffs.h"
#include "../../../Entities/Include/Player/PlayerStatus.h"


GamePlayControlArea::GamePlayControlArea()
{
    controlArea.setOrigin({
        controlArea.getTexture().getSize().x / 2.f,
        controlArea.getTexture().getSize().y / 2.f
    });
    controlArea.setPosition(controlAreaOffScreenPosition);

    // Set the button positions
    buttons.num2.setFramePosition({
        controlArea.getTexture().getSize().x / 2.f - 40.f,
        97.5f
    });
    buttons.xboxBButton.setPosition(buttons.num2.getPosition());
    buttons.dualsenseCircleButton.setPosition(buttons.num2.getPosition());

    buttons.num1.setFramePosition({
        buttons.num2.frame.getPosition().x - 80.f,
        97.5f
    });
    buttons.xboxAButton.setPosition(buttons.num1.getPosition());
    buttons.dualsenseXButton.setPosition(buttons.num1.getPosition());

    buttons.num3.setFramePosition({
        controlArea.getTexture().getSize().x / 2.f + 40.f,
        97.5f
    });
    buttons.xboxXButton.setPosition(buttons.num3.getPosition());
    buttons.dualsenseSquareButton.setPosition(buttons.num3.getPosition());

    buttons.num4.setFramePosition({
        buttons.num3.frame.getPosition().x + 80.f,
        97.5f
    });
    buttons.xboxYButton.setPosition(buttons.num4.getPosition());
    buttons.dualsenseTriangleButton.setPosition(buttons.num4.getPosition());

    buttons.spaceBar.setFramePosition({
        buttons.num4.frame.getPosition().x + 60.f,
        97.5f
    });
    buttons.xboxRightTrigger.setPosition(buttons.spaceBar.getPosition());
    buttons.dualsenseRightTrigger.setPosition(buttons.spaceBar.getPosition());

    // Set the lives and nukes
    lives.frame.setPosition({
        controlArea.getTexture().getSize().x / 2.f - 255.f,
        40.f
    });
    lives.setPosition({
        lives.frame.getPosition().x + 70.f,
        lives.frame.getPosition().y
    });
    lives.setScale({0.65f, 0.65f});
    lives.setRotation(sf::radians(-PI / 2));

    nukes.frame.setPosition({
        controlArea.getTexture().getSize().x / 2.f + 255.f,
        40.f
    });
    nukes.setPosition({
        nukes.frame.getPosition().x - 70.f,
        nukes.frame.getPosition().y
    });
    nukes.setScale({0.75f, 0.75f});
}


GamePlayControlArea::ResourceCounter::ResourceCounter(const sf::Texture& texture) : Sprite(texture)
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


void GamePlayControlArea::transitionIn()
{
    if (isTransitioningIn)
    {
        // Increment for position ease
        transitionTime += GameRoot::instance().deltaTime;

        // Ease position
        controlArea.setPosition(Extensions::easeOutCubic(controlAreaOffScreenPosition, controlAreaOnScreenPosition, transitionTime / TRANSITION_DURATION));

        // Set stopping point
        if (transitionTime > TRANSITION_DURATION)
        {
            isTransitioningIn = false;
            transitionTime = 0.f;
            controlArea.setPosition(controlAreaOnScreenPosition);
        }
    }
}


void GamePlayControlArea::transitionOut()
{
    if (isTransitioningOut)
    {
        // Increment for position ease
        transitionTime += GameRoot::instance().deltaTime;

        // Ease position
        controlArea.setPosition(Extensions::easeInCubic(controlAreaOnScreenPosition, controlAreaOffScreenPosition, transitionTime / TRANSITION_DURATION));

        // Set stopping point
        if (transitionTime > TRANSITION_DURATION)
        {
            isTransitioningOut = false;
            transitionTime = 0.f;
            controlArea.setPosition(controlAreaOffScreenPosition);
        }
    }
}


void GamePlayControlArea::drawToBlur()
{
    controlAreaTexture.clear(sf::Color::Transparent);
    Buffs::instance().drawBuffs(controlAreaTexture);
    controlAreaTexture.display();
    GaussianBlur::instance().drawToBase(controlArea);

    Buffs::instance().drawBuffDrops();
}


void GamePlayControlArea::drawToScreen()
{
    controlAreaTexture.clear(sf::Color::Transparent);
    drawLivesAndNukes();
    buttons.draw(controlAreaTexture);
    Buffs::instance().drawText(controlAreaTexture);
    controlAreaTexture.display();
    GameRoot::instance().renderWindow.draw(controlArea);
}


void GamePlayControlArea::drawLivesAndNukes()
{
    // Draw lives and frame
    lives.frame.setColor({255, 255, 255, static_cast<std::uint8_t>(255 * GameRoot::instance().frameUIOpacity)});
    controlAreaTexture.draw(lives.frame);
    for (int i = 0; i < PlayerStatus::instance().lives; i++)
    {
        sf::Sprite nextSprite = {lives};
        nextSprite.setPosition({ nextSprite.getPosition().x - i * 35, nextSprite.getPosition().y });
        controlAreaTexture.draw(nextSprite);
    }

    // Draw bombs and frame
    nukes.frame.setColor({255, 255, 255, static_cast<std::uint8_t>(255 * GameRoot::instance().frameUIOpacity)});
    controlAreaTexture.draw(nukes.frame);
    for (int i = 0; i < Nukes::instance().count; i++)
    {
        sf::Sprite nextSprite = {nukes};
        nextSprite.setPosition({ nextSprite.getPosition().x + i * 35, nextSprite.getPosition().y });
        controlAreaTexture.draw(nextSprite);
    }
}

