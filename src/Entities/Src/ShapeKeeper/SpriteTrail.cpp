#include "../../Include/ShapeKeeper/SpriteTrail.h"


SpriteTrail::SpriteTrail(sf::Sprite* spriteToTrail)
{
    this->spriteToTrail = spriteToTrail;
    trail.fill({});
}


void SpriteTrail::Trail::reset()
{
    elapsed = 0.f;
    alpha = MAX_ALPHA;
    position = {0.f, 0.f};
    isActive = false;
}


void SpriteTrail::Trail::activate(const sf::Vector2f& position)
{
    this->position = position;
    elapsed = DURATION;
    isActive = true;
}


void SpriteTrail::reset()
{
    for (int i = 0; i < SIZE; i++)
        trail.at(i).reset();
}


void SpriteTrail::update()
{
    // Check time for a new one
    createNewTrailTime -= GameRoot::instance().deltaTime;
    if (createNewTrailTime < 0.f)
    {
        createNewTrailTime = CREATE_NEW_TRAIL_DURATION;

        // Find a trail that is not active
        for (int i = 0; i < SIZE; i++)
            if (!trail.at(i).isActive)
            {
                trail.at(i).activate(spriteToTrail->getPosition());
                break;
            }
    }

    // Update trails
    for (int i = 0; i < SIZE; i++)
    {
        if (trail.at(i).isActive)
        {
            trail.at(i).elapsed -= GameRoot::instance().deltaTime;

            trail.at(i).alpha = static_cast<std::uint8_t>(Trail::MAX_ALPHA * (trail.at(i).elapsed / trail.at(i).DURATION));

            if (trail.at(i).elapsed < 0.f)
                trail.at(i).reset();
        }
    }
}


void SpriteTrail::draw()
{
    if (spriteToTrail != nullptr)
        for (int i = 0; i < SIZE; i++)
            if (trail.at(i).isActive)
            {
                sf::Sprite sprite {*spriteToTrail};
                sprite.setColor({spriteToTrail->getColor().r, spriteToTrail->getColor().g, spriteToTrail->getColor().b, trail.at(i).alpha});
                sprite.setPosition(trail.at(i).position);
                GaussianBlur::instance().drawToBase(sprite);
            }
}
