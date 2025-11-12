#include "../Include/HighscoreArea.h"
#include "../../GameRoot.h"
#include "../../Core/Include/Extensions.h"
#include "../../Entities/Include/Player/PlayerStatus.h"


HighscoreArea::HighscoreArea(const bool withTransition, const sf::Vector2f& onScreenPosition, const sf::Vector2f& offScreenPosition)
{
    this->withTransition = withTransition;
    this->onScreenPosition = onScreenPosition;
    this->offScreenPosition = offScreenPosition;
    containerTexture.setSmooth(true);
    containerSprite.setOrigin(containerSprite.getLocalBounds().getCenter());
    containerSprite.setPosition(onScreenPosition);
    label.setPosition({containerTexture.getTexture().getSize().x / 2.f, 40.f});
    label.setStyle(sf::Text::Bold);
    label.setOrigin(label.getLocalBounds().getCenter());
    highScoreText.setPosition({label.getPosition().x, label.getPosition().y + 55.f});
    highScoreText.setStyle(sf::Text::Bold);
}


void HighscoreArea::startOnScreen()
{
    containerSprite.setPosition(onScreenPosition);
}


void HighscoreArea::startOffScreen()
{
    containerSprite.setPosition(offScreenPosition);
}


void HighscoreArea::transitionIn()
{
    if (!withTransition || !isTransitioningIn)
        return;

    transitionTime += GameRoot::instance().deltaTime;

    containerSprite.setPosition(Extensions::easeOutCubic(offScreenPosition, onScreenPosition, transitionTime / TRANSITION_DURATION));

    if (transitionTime > TRANSITION_DURATION)
    {
        isTransitioningIn = false;
        transitionTime = 0.f;
        containerSprite.setPosition(onScreenPosition);
    }
}


void HighscoreArea::transitionOut()
{
    if (!withTransition || !isTransitioningOut)
        return;

    transitionTime += GameRoot::instance().deltaTime;

    containerSprite.setPosition(Extensions::easeInCubic(onScreenPosition, offScreenPosition, transitionTime / TRANSITION_DURATION));

    if (transitionTime > TRANSITION_DURATION)
    {
        isTransitioningOut = false;
        transitionTime = 0.f;
        containerSprite.setPosition(offScreenPosition);
    }
}


void HighscoreArea::update()
{
    transitionIn();
    transitionOut();
}


void HighscoreArea::alignHighscoreText()
{
    highScoreText.setString(Extensions::formatNumberWithCommas(PlayerStatus::instance().highScore));
    highScoreText.setOrigin(highScoreText.getLocalBounds().getCenter());
}


void HighscoreArea::drawToScreen()
{
    alignHighscoreText();

    containerTexture.clear(sf::Color::Transparent);
    containerTexture.draw(label);
    containerTexture.draw(highScoreText);
    containerTexture.display();
    GameRoot::instance().renderWindow.draw(containerSprite);
}


void HighscoreArea::drawToTexture(sf::RenderTexture& texture)
{
    alignHighscoreText();

    containerTexture.clear(sf::Color::Transparent);
    containerTexture.draw(label);
    containerTexture.draw(highScoreText);
    containerTexture.display();
    texture.draw(containerSprite);
}
