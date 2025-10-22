#ifndef PARTICLES_H
#define PARTICLES_H
#include <array>
#include <random>
#include "../../Content/Include/Art.h"
#include "SFML/Graphics/Sprite.hpp"


enum ParticleType
{
    IgnoreGravity,
    DontIgnoreGravity
};

enum ParticleSize
{
    Spark,
    Explosion,
    Massive
};

class Particles {
private:
    class Particle
    {
    private:
        union
        {
            struct
            {
                float xVelocity;
                float yVelocity;
                float duration;
                float percentLife;
            };

            Particle *next {nullptr};
        };

        sf::Sprite particleSprite {Art::instance().particle};
        sf::Sprite glowSprite {Art::instance().particleGlow};
        ParticleType particleType = DontIgnoreGravity;
        ParticleSize particleSize = Explosion;

        void move(sf::Vector2f);
        void setPosition(sf::Vector2f);
        void setScale(sf::Vector2f);
        void setRotation(sf::Angle);
        void resetColors();

    public:
        Particle();

        bool isActive = false;
        bool withGlow = false;

        Particle *getNext() const;
        void setNext(Particle *);
        void activate(float, ParticleType, ParticleSize, sf::Vector2f, sf::Vector2f, sf::Color);
        void activate(float, ParticleType, ParticleSize, sf::Vector2f, sf::Vector2f, sf::Color, sf::Color);
        void reset();
        bool animate();
        void draw() const;
    };

    static constexpr int MAX_PARTICLE_COUNT = 16'384;
    Particle *firstAvailable {nullptr};

public:
    Particles();

    static Particles &instance()
    {
        static auto *instance = new Particles;
        return *instance;
    }

    std::array<Particle, MAX_PARTICLE_COUNT> particles {};
    std::default_random_engine randEngine {std::random_device{}()};

    void create(float, ParticleType, ParticleSize, sf::Vector2f, sf::Vector2f, sf::Color);
    void create(float, ParticleType, ParticleSize, sf::Vector2f, sf::Vector2f, sf::Color, sf::Color);
    void update();
    void draw() const;
};



#endif //PARTICLES_H
