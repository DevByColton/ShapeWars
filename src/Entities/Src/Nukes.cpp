#include "../Include/Nukes.h"
#include "../../Content/Include/GaussianBlur.h"
#include "../../Grid/Grid.h"
#include "../../PlayerStatus/PlayerStatus.h"
#include "../../System/Include/Extensions.h"


Nukes::Nukes()
{
    nukeCircle.setOutlineThickness(2.5f);
    nukeCircle.setOutlineColor(nukeColor);
    nukeCircle.setFillColor(sf::Color::Transparent);
}


void Nukes::reset()
{
    radius = initialRadius;
    velocity = initialVelocity;
    nukeCircle.setRadius(radius);
    nukeCircle.setPosition({0.f, 0.f});
    isDetonating = false;
}


void Nukes::resetNukeCount()
{
    count = 3;
}


void Nukes::markDetonate(const sf::Vector2f &fromPosition)
{
    if (!PlayerStatus::instance().isDead() && !isDetonating && count > 0)
    {
        isDetonating = true;
        count -= 1;
        nukeCircle.setPosition(fromPosition);
        Grid::instance().applyExplosiveForce(fromPosition, 400.f, 450.f, 0.9f);
    }
}


sf::Vector2f Nukes::getPosition() const
{
    return nukeCircle.getPosition();
}


bool Nukes::update()
{
    if (isDetonating)
    {
        radius = nukeCircle.getRadius() + velocity;
        nukeCircle.setRadius(radius);
        nukeCircle.setOrigin(nukeCircle.getGeometricCenter());

        // When all four corners of the screen are contained that means the circle is bigger than the screen, reset
        const float radii = nukeCircle.getRadius() * nukeCircle.getRadius();
        const bool topLeftContained = Extensions::distanceSquared(nukeCircle.getPosition(), GameRoot::instance().topLeftCorner) <= radii;
        const bool topRightContained = Extensions::distanceSquared(nukeCircle.getPosition(), GameRoot::instance().topRightCorner) <= radii;
        const bool bottomRightContained = Extensions::distanceSquared(nukeCircle.getPosition(), GameRoot::instance().bottomRightCorner) <= radii;
        const bool bottomLeftContained = Extensions::distanceSquared(nukeCircle.getPosition(), GameRoot::instance().bottomLeftCorner) <= radii;
        if (topLeftContained && topRightContained && bottomRightContained && bottomLeftContained)
        {
            reset();
            return true;
        }
    }

    return false;
}


void Nukes::draw() const
{
    if (isDetonating)
        GaussianBlur::instance().drawToBase(nukeCircle);
}
