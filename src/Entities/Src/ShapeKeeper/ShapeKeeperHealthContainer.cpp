#include "../../Include/ShapeKeeper/ShapeKeeperHealthContainer.h"
#include "../../../Content/Include/GaussianBlur.h"
#include "../../../System/Include/Extensions.h"


ShapeKeeperHealthContainer::ShapeKeeperHealthContainer()
{
    // Wrapper for all the health bars and text
    renderTexture.setSmooth(true);
    sprite.setPosition(healthWrapperOffScreenPosition);
    sprite.setOrigin({
        renderTexture.getSize().x / 2.f,
        renderTexture.getSize().y / 2.f
    });

    // Shape keeper header font
    headerText.setOrigin({
        headerText.getLocalBounds().size.x / 2.f,
        headerText.getLocalBounds().size.y / 2.f
    });
    headerText.setPosition({sprite.getOrigin().x, 100.f});
}


void ShapeKeeperHealthContainer::transitionIn()
{
    if (isTransitioningIn)
    {
        // Increment for position ease
        transitionTime += GameRoot::instance().deltaTime;

        // Ease position
        sprite.setPosition(Extensions::easeOutBack(healthWrapperOffScreenPosition, healthWrapperOnScreenPosition, transitionTime / TRANSITION_DURATION));

        // Set stopping point
        if (transitionTime > TRANSITION_DURATION)
        {
            isTransitioningIn = false;
            transitionTime = 0.f;
            sprite.setPosition(healthWrapperOnScreenPosition);
        }
    }
}


void ShapeKeeperHealthContainer::transitionOut()
{
    if (isTransitioningOut)
    {
        // Increment for position ease
        transitionTime += GameRoot::instance().deltaTime;

        // Ease position
        sprite.setPosition(Extensions::easeBackIn(healthWrapperOnScreenPosition, healthWrapperOffScreenPosition, transitionTime / TRANSITION_DURATION));

        // Set stopping point
        if (transitionTime > TRANSITION_DURATION)
        {
            isTransitioningOut = false;
            transitionTime = 0.f;
            sprite.setPosition(healthWrapperOffScreenPosition);
        }
    }
}


void ShapeKeeperHealthContainer::draw()
{
    renderTexture.clear(sf::Color::Transparent);
    renderTexture.draw(frame);
    renderTexture.draw(top.background);
    renderTexture.draw(top);
    renderTexture.draw(middleLeft.background);
    renderTexture.draw(middleLeft);
    renderTexture.draw(middleRight.background);
    renderTexture.draw(middleRight);
    renderTexture.draw(bottomLeft.background);
    renderTexture.draw(bottomLeft);
    renderTexture.draw(bottomRight.background);
    renderTexture.draw(bottomRight);
    renderTexture.draw(core.background);
    renderTexture.draw(core);
    renderTexture.display();
    GaussianBlur::instance().drawToBase(sprite);
}


void ShapeKeeperHealthContainer::drawText()
{
    frame.setColor({255, 255, 255, static_cast<std::uint8_t>(255 * GameRoot::instance().frameUIOpacity)});

    renderTexture.clear(sf::Color::Transparent);
    renderTexture.draw(headerText);
    renderTexture.display();
    GameRoot::instance().renderWindow.draw(sprite);
}
