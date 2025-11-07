#ifndef ACTIVEMENUOPTIONINDICATOR_H
#define ACTIVEMENUOPTIONINDICATOR_H
#include "../../Content/Include/Art.h"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/RenderTexture.hpp"


struct OptionIndicator
{
    struct ActiveMenuOptionIndicator final : sf::Sprite
    {
        explicit ActiveMenuOptionIndicator(const sf::Texture& texture);

        sf::Vector2f previousPosition {0.f, 0.f};
        sf::Vector2f targetPosition {0.f, 0.f};

        void setActive(const sf::Vector2f& targetPosition);
        void transition(float time);
    };

    static constexpr float INDICATORS_TRANSITION_DURATION = 0.075f;
    float indicatorsTransitionTime = 0.f;
    bool isActiveOptionIndicatorTransitioning = false;
    ActiveMenuOptionIndicator left {Art::instance().shapeKeeperCore};
    ActiveMenuOptionIndicator right {Art::instance().shapeKeeperCore};

    void setActive(const sf::Vector2f& leftTargetPosition, const sf::Vector2f& rightTargetPosition);
    void update();
    void draw(sf::RenderTexture& renderTexture) const;
};



#endif //ACTIVEMENUOPTIONINDICATOR_H
