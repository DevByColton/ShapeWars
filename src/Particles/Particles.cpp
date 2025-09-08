#include <cmath>
#include "Particles.h"
#include "../GameRoot.h"
#include "../Content/Include/GaussianBlur.h"
#include "../Entities/Include/BlackHoles.h"
#include "../System/Include/Extensions.h"


Particles::Particles()
{
    // Set the head of the particle list
    firstAvailable = &particles.at(0);

    // Link them up
    for (int i = 0; i < MAX_PARTICLE_COUNT - 1; i++)
        particles.at(i).setNext(&particles.at(i + 1));

    // The last one in the list terminates
    particles.at(MAX_PARTICLE_COUNT - 1).setNext(nullptr);
}


Particles::Particle::Particle()
{
    // Set the particle sprite origin to the middle of the sprite
    float x = Art::instance().particle.getSize().x / 2.0f;
    float y = Art::instance().particle.getSize().y / 2.0f;
    particleSprite.setOrigin({x, y});

    // Set the particle sprite origin to the middle of the sprite
    float xg = Art::instance().particleGlow.getSize().x / 2.0f;
    float yg = Art::instance().particleGlow.getSize().y / 2.0f;
    glowSprite.setOrigin({xg, yg});
}


Particles::Particle* Particles::Particle::getNext() const
{
    return next;
}


void Particles::Particle::setNext(Particle *next)
{
    this->next = next;
}


void Particles::Particle::move(sf::Vector2f offset)
{
    particleSprite.move(offset);
    glowSprite.move(offset);
}


void Particles::Particle::setPosition(const sf::Vector2f position)
{
    particleSprite.setPosition(position);
    glowSprite.setPosition(position);
}


void Particles::Particle::setScale(const sf::Vector2f scale)
{
    particleSprite.setScale(scale);
    glowSprite.setScale(scale);
}


void Particles::Particle::setRotation(const sf::Angle angle)
{
    particleSprite.setRotation(angle);
    glowSprite.setRotation(angle);
}


void Particles::Particle::resetColors()
{
    particleSprite.setColor(sf::Color::White);
    glowSprite.setColor(sf::Color::White);
}


void Particles::Particle::reset()
{
    xVelocity = 0.0;
    yVelocity = 0.0;
    duration = 0.0;
    percentLife = 0.0;
    particleSprite.setColor(sf::Color::White);
    setPosition({0.0, 0.0});
    setRotation(sf::Angle::Zero);
    setScale({1.0f, 1.0f});
    resetColors();
    particleType = DontIgnoreGravity;
    particleSize = Explosion;
    withGlow = false;
    isActive = false;
}


void Particles::Particle::activate(const float duration,
                                   const ParticleType particleType,
                                   const ParticleSize particleSize,
                                   const sf::Vector2f position,
                                   const sf::Vector2f initialVelocity,
                                   const sf::Color color)
{
    this->particleType = particleType;
    this->particleSize = particleSize;
    this->duration = duration;
    percentLife = 1.0;
    xVelocity = initialVelocity.x;
    yVelocity = initialVelocity.y;
    setPosition(position);
    particleSprite.setColor(color);
    isActive = true;
}


void Particles::Particle::activate(const float duration,
                                   const ParticleType particleType,
                                   const ParticleSize particleSize,
                                   const sf::Vector2f position,
                                   const sf::Vector2f initialVelocity,
                                   const sf::Color particleColor,
                                   const sf::Color glowColor)
{
    this->particleType = particleType;
    this->particleSize = particleSize;
    this->duration = duration;
    percentLife = 1.0;
    xVelocity = initialVelocity.x;
    yVelocity = initialVelocity.y;
    setPosition(position);
    particleSprite.setColor(particleColor);
    glowSprite.setColor(glowColor);
    withGlow = true;
    isActive = true;
}


void Particles::create(const float duration,
                       const ParticleType particleType,
                       const ParticleSize particleSize,
                       const sf::Vector2f position,
                       const sf::Vector2f initialVelocity,
                       const sf::Color color)
{

    assert(firstAvailable != nullptr);

    Particle *newParticle = firstAvailable;
    if (newParticle->getNext() != nullptr)
    {
        firstAvailable = newParticle->getNext();
        newParticle->activate(duration, particleType, particleSize, position, initialVelocity, color);
    }
}


void Particles::create(const float duration,
                       const ParticleType particleType,
                       const ParticleSize particleSize,
                       const sf::Vector2f position,
                       const sf::Vector2f initialVelocity,
                       const sf::Color particleColor,
                       const sf::Color glowColor)
{

    assert(firstAvailable != nullptr);

    Particle *newParticle = firstAvailable;
    if (newParticle->getNext() != nullptr)
    {
        firstAvailable = newParticle->getNext();
        newParticle->activate(duration, particleType, particleSize, position, initialVelocity, particleColor, glowColor);
    }
}



float Particles::randomStartingSpeed(const float size, const float min, const float max)
{
    std::uniform_real_distribution particleSpeed {min, max};
    return size * (1.0f - 1.0f / particleSpeed(randEngine));
}



void Particles::update()
{
    for (int i = 0; i < MAX_PARTICLE_COUNT; i++)
        if (particles.at(i).isActive)
            if (particles.at(i).animate())
            {
                particles.at(i).setNext(firstAvailable);
                firstAvailable = &particles.at(i);
            }
}


bool Particles::Particle::animate()
{
    sf::Vector2f velocity {xVelocity, yVelocity};
    const float speed = velocity.length();

    move(velocity);

    // Set the alpha by the lower of the speed or percent life left, or 1 if they are bigger than 1
    float alpha = std::min(1.0f, std::min(percentLife * 2, speed));
    alpha *= alpha;
    particleSprite.setColor({particleSprite.getColor().r, particleSprite.getColor().g, particleSprite.getColor().b, static_cast<uint8_t>(255.0f * alpha)});
    glowSprite.setColor({glowSprite.getColor().r, glowSprite.getColor().g, glowSprite.getColor().b, static_cast<uint8_t>(255.0f * alpha)});

    // Update the x scale of the particle
    float scaleX = 0.0;
    switch (particleSize)
    {
    case Spark:
        scaleX = std::min(std::min(1.0f, 0.1f * speed + 0.1f), alpha);
        break;
    case Explosion:
        scaleX = std::min(std::min(1.0f, 0.2f * speed + 0.1f), alpha);
        break;
    case Massive:
        scaleX = std::min(std::min(1.0f, 0.3f * speed + 0.1f), alpha);
        break;
    }
    setScale({scaleX, particleSprite.getScale().y});

    // Update the angle of the particle
    setRotation(sf::radians(toAngle(velocity)));

    // Update the velocity, make sure they collide with the bounds of the screen
    if (particleSprite.getPosition().x < 0.0f)
        velocity.x = std::abs(velocity.x);
    else if (particleSprite.getPosition().x > GameRoot::instance().windowSizeF.x)
        velocity.x = -std::abs(velocity.x);

    if (particleSprite.getPosition().y < 0.0f)
        velocity.y = std::abs(velocity.y);
    else if (particleSprite.getPosition().y > GameRoot::instance().windowSizeF.y)
        velocity.y = -std::abs(velocity.y);

    // Apply black hole force on the particle
    if (particleType == DontIgnoreGravity)
    {
        const auto &blackHoles = BlackHoles::instance().blackHoles;

        for (int i = 0; i < BlackHoles::MAX_BLACK_HOLE_COUNT; i++)
            if (blackHoles.at(i).isActive)
            {
                sf::Vector2f direction = blackHoles.at(i).getPosition() - particleSprite.getPosition();
                const float distance = direction.length();
                const sf::Vector2f n = direction / distance;
                velocity.x += 10'000.0f * n.x / (distance * distance + 10'000.0f);
                velocity.y += 10'000.0f * n.y / (distance * distance + 10'000.0f);

                // Add tangential acceleration for nearby particles
                if (distance < 400.0f)
                {
                    const sf::Vector2f perpendicular = {n.y, -n.x};
                    velocity.x += 45 * perpendicular.x / (distance + 100.0f);
                    velocity.y += 45 * perpendicular.y / (distance + 100.0f);
                }
            }
    }

    // Update the particle life
    percentLife -= 1.0f / duration;

    // If the percent life is expired or the velocity is very, very small, reset the particle
    // NOTE: Denormalized floats cause performance issues
    if (percentLife < 0.0f || std::abs(velocity.x) + std::abs(velocity.y) < 0.0001f)
    {
        reset();
        return true;
    }

    // Apply friction
    switch (particleSize)
    {
    case Spark:
        velocity *= 0.92f;
        break;
    case Explosion:
        velocity *= 0.94f;
        break;
    case Massive:
        velocity *= 0.98f;
        break;
    }

    // Apply the new velocity
    xVelocity = velocity.x;
    yVelocity = velocity.y;

    // Ah, ah, ah, stayin alive
    return false;
}



void Particles::draw() const
{
    for (int i = 0; i < MAX_PARTICLE_COUNT; i++)
        particles.at(i).draw();
}


void Particles::Particle::draw() const
{
    if (isActive)
    {
        GaussianBlur::instance().drawToBase(particleSprite);

        if (withGlow)
            GaussianBlur::instance().drawToBase(glowSprite);
    }
}


