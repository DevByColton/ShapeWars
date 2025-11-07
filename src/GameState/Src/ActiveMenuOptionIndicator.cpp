#include "../Include/ActiveMenuOptionIndicator.h"
#include "../../GameRoot.h"
#include "../../Core/Include/Extensions.h"


OptionIndicator::ActiveMenuOptionIndicator::ActiveMenuOptionIndicator(const sf::Texture& texture) : Sprite(texture)
{
    setOrigin(getLocalBounds().getCenter());
    setScale({0.4f, 0.4f});
}


void OptionIndicator::ActiveMenuOptionIndicator::transition(const float time)
{
    setPosition(Extensions::easeOutCubic(previousPosition, targetPosition, time));
}


void OptionIndicator::ActiveMenuOptionIndicator::setActive(const sf::Vector2f& targetPosition)
{
    previousPosition = getPosition();
    this->targetPosition = targetPosition;
}


void OptionIndicator::setActive(const sf::Vector2f& leftTargetPosition, const sf::Vector2f& rightTargetPosition)
{
    left.setActive(leftTargetPosition);
    right.setActive(rightTargetPosition);
    isActiveOptionIndicatorTransitioning = true;
}


void OptionIndicator::update()
{
    if (isActiveOptionIndicatorTransitioning)
    {
        indicatorsTransitionTime += GameRoot::instance().deltaTime;

        const float time = indicatorsTransitionTime / INDICATORS_TRANSITION_DURATION;
        left.transition(time);
        right.transition(time);

        // Set stopping point
        if (indicatorsTransitionTime > INDICATORS_TRANSITION_DURATION)
        {
            isActiveOptionIndicatorTransitioning = false;
            indicatorsTransitionTime = 0.f;
            left.setPosition(left.targetPosition);
            right.setPosition(right.targetPosition);
        }
    }

    left.rotate(sf::radians(2.f * GameRoot::instance().deltaTime));
    right.rotate(sf::radians(-2.f * GameRoot::instance().deltaTime));
}


void OptionIndicator::draw(sf::RenderTexture& renderTexture) const
{
    renderTexture.draw(left);
    renderTexture.draw(right);
}
