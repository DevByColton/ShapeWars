#include "../Include/LivesAndNukes.h"
#include "../../GameRoot.h"
#include "../../Entities/Include/Nukes.h"
#include "../../PlayerStatus/Include/PlayerStatus.h"
#include "../../System/Include/Extensions.h"


LivesAndNukes::LivesAndNukes()
{
    // Set the lives frame position
    livesFrame.setOrigin({
        livesFrame.getTexture().getSize().x / 2.f,
        livesFrame.getTexture().getSize().y / 2.f
    });
    livesFrame.setPosition({
        GameRoot::instance().windowSizeF.x / 2.f - 255.f,
        GameRoot::instance().windowSizeF.y - 80.f
    });
    livesSprite.setOrigin({
        livesSprite.getTexture().getSize().x / 2.f,
        livesSprite.getTexture().getSize().y / 2.f
    });
    livesSprite.setPosition({
        livesFrame.getPosition().x + 70.f,
        livesFrame.getPosition().y
    });
    livesSprite.setScale({0.65f, 0.65f});
    livesSprite.setRotation(sf::radians(-PI / 2));

    // Set the bombs frame position
    nukesFrame.setOrigin({
        nukesFrame.getTexture().getSize().x / 2.f,
        nukesFrame.getTexture().getSize().y / 2.f
    });
    nukesFrame.setPosition({
        GameRoot::instance().windowSizeF.x / 2.f + 255.f,
        GameRoot::instance().windowSizeF.y - 80.f
    });
    nukeSprite.setOrigin({
    nukeSprite.getTexture().getSize().x / 2.f,
    nukeSprite.getTexture().getSize().y / 2.f
    });
    nukeSprite.setPosition({
        nukesFrame.getPosition().x - 70.f,
        nukesFrame.getPosition().y
    });
    nukeSprite.setScale({0.75f, 0.75f});
}


void LivesAndNukes::draw()
{
    // Draw lives and frame
    livesFrame.setColor({255, 255, 255, static_cast<std::uint8_t>(255 * GameRoot::instance().frameUIOpacity)});
    GameRoot::instance().renderWindow.draw(livesFrame);
    for (int i = 0; i < PlayerStatus::instance().lives; i++)
    {
        sf::Sprite nextSprite = livesSprite;
        nextSprite.setPosition({ nextSprite.getPosition().x - i * 35, nextSprite.getPosition().y });
        GameRoot::instance().renderWindow.draw(nextSprite);
    }

    // Draw bombs and frame
    nukesFrame.setColor({255, 255, 255, static_cast<std::uint8_t>(255 * GameRoot::instance().frameUIOpacity)});
    GameRoot::instance().renderWindow.draw(nukesFrame);
    for (int i = 0; i < Nukes::instance().count; i++)
    {
        sf::Sprite nextSprite = nukeSprite;
        nextSprite.setPosition({ nextSprite.getPosition().x + i * 35, nextSprite.getPosition().y });
        GameRoot::instance().renderWindow.draw(nextSprite);
    }
}
