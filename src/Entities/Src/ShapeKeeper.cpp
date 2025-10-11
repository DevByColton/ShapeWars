#include "../Include/ShapeKeeper.h"
#include "../../Content/Include/GaussianBlur.h"


ShapeKeeper::ShapeKeeperCore::ShapeKeeperCore(const sf::Texture& texture) : Sprite(texture)
{
    setOrigin({getTexture().getSize().x / 2.f, getTexture().getSize().y / 2.f});

    setPosition({
        GameRoot::instance().windowSizeF.x / 2.f,
        GameRoot::instance().windowSizeF.y / 2.f
    });

    // Start the core rotated
    setRotation(sf::radians(sf::degrees(324.f).asRadians()));
}


ShapeKeeper::ShapeKeeperBodyPart::ShapeKeeperBodyPart(const sf::Texture& texture, const float coreXOffset, const float coreYOffset) : Sprite(texture)
{
    this->coreXOffset = coreXOffset;
    this->coreYOffset = coreYOffset;
    setOrigin({getTexture().getSize().x / 2.f, getTexture().getSize().y / 2.f});
}


ShapeKeeper::ShapeKeeperHealthBar::ShapeKeeperHealthBar(const sf::Texture& texture, const sf::Vector2f &position) : Sprite(texture), background(texture)
{
    setPosition(position);
    setOrigin({getTexture().getSize().x / 2.f, getTexture().getSize().y / 2.f});
    background.setPosition(getPosition());
    background.setOrigin(getOrigin());
    background.setColor({getColor().r, getColor().g, getColor().b, 64});

    setTextureRect({{0,0}, {40, 40}});
}


ShapeKeeper::ShapeKeeper()
{
    // Wrapper for all of the health bars and text
    healthWrapper.setSmooth(true);
    healthWrapperSprite.setPosition({GameRoot::instance().windowSizeF.x / 2.f, 50.f});
    healthWrapperSprite.setOrigin({
        healthWrapper.getSize().x / 2.f,
        healthWrapper.getSize().y / 2.f
    });

    // Shape keeper header font
    shapeKeeperText.setOrigin({
        shapeKeeperText.getLocalBounds().size.x / 2.f,
        shapeKeeperText.getLocalBounds().size.y / 2.f
    });
    shapeKeeperText.setPosition({healthWrapperSprite.getOrigin().x, 100.f});

    // Parts
    top.core = &core;
    top.update();

    middleLeft.core = &core;
    middleLeft.update();

    middleRight.core = &core;
    middleRight.update();

    bottomLeft.core = &core;
    bottomLeft.update();

    bottomRight.core = &core;
    bottomRight.update();
}


void ShapeKeeper::ShapeKeeperCore::update()
{
    // TODO: movement and stuff
}


void ShapeKeeper::ShapeKeeperBodyPart::update()
{
    setPosition({core->getPosition().x + coreXOffset, core->getPosition().y + coreYOffset});
}


void ShapeKeeper::ShapeKeeperHealthBar::update()
{
    //todo: update health bar texture rect to reflect health change
}


void ShapeKeeper::update()
{
    top.update();
    topHealthBar.update();
    middleLeft.update();
    middleLeftHealthBar.update();
    middleRight.update();
    middleRightHealthBar.update();
    bottomLeft.update();
    bottomLeftHealthBar.update();
    bottomRight.update();
    bottomRightHealthBar.update();
    core.update();
    coreHealthBar.update();
}


void ShapeKeeper::draw()
{
    GaussianBlur::instance().drawToBase(top);
    GaussianBlur::instance().drawToBase(middleLeft);
    GaussianBlur::instance().drawToBase(middleRight);
    GaussianBlur::instance().drawToBase(bottomLeft);
    GaussianBlur::instance().drawToBase(bottomRight);
    GaussianBlur::instance().drawToBase(core);

    shapeKeeperHealthFrame.setColor({255, 255, 255, static_cast<std::uint8_t>(255 * GameRoot::instance().frameUIOpacity)});

    healthWrapper.clear(sf::Color::Transparent);
    healthWrapper.draw(shapeKeeperHealthFrame);
    healthWrapper.draw(topHealthBar.background);
    healthWrapper.draw(topHealthBar);
    healthWrapper.draw(middleLeftHealthBar.background);
    healthWrapper.draw(middleLeftHealthBar);
    healthWrapper.draw(middleRightHealthBar.background);
    healthWrapper.draw(middleRightHealthBar);
    healthWrapper.draw(bottomLeftHealthBar.background);
    healthWrapper.draw(bottomLeftHealthBar);
    healthWrapper.draw(bottomRightHealthBar.background);
    healthWrapper.draw(bottomRightHealthBar);
    healthWrapper.draw(coreHealthBar.background);
    healthWrapper.draw(coreHealthBar);
    healthWrapper.display();
    GaussianBlur::instance().drawToBase(healthWrapperSprite);
}


void ShapeKeeper::drawFont()
{
    healthWrapper.clear(sf::Color::Transparent);
    healthWrapper.draw(shapeKeeperText);
    healthWrapper.display();
    GameRoot::instance().renderWindow.draw(healthWrapperSprite);
}

