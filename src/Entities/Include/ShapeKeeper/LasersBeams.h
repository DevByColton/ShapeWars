#ifndef LASERSBEAMS_H
#define LASERSBEAMS_H
#include "ShapeKeeperBodyPart.h"


struct LasersBeams {
    explicit LasersBeams(ShapeKeeperCore* core);

    enum AnimationState
    {
        Idle,
        Entering,
        Active,
        Leaving
    };

    struct Laser
    {
        Laser() = default;
        explicit Laser(const sf::Vector2f& position);

        static constexpr int COLLIDERS_SIZE = 22;
        static constexpr float LASER_EXPAND_DURATION = 0.15f;
        static constexpr float LASER_COLLAPSE_DURATION = 0.1f;
        float laserExpandTime = 0.f;
        float laserCollapseTime = LASER_COLLAPSE_DURATION;
        AnimationState animationState = Idle;
        sf::Sprite laser {Art::instance().laser};
        sf::Color blastColor {255, 205, 191, 255};
        std::array<sf::CircleShape, COLLIDERS_SIZE> colliders;

        void reset();
        void updateAnimation(const sf::Vector2f& laserEndPoint);
    };

    static constexpr int LASERS_SIZE = 5;
    float laserCoreOffset = 0.f;
    float baseBlastOffset = 0.f;
    ShapeKeeperCore* core {nullptr};
    sf::Angle laserRotation = sf::Angle::Zero;
    std::array<Laser, LASERS_SIZE> lasers;

    // Laser spawn
    static constexpr float DEFAULT_TIME_UNTIL_LASER = 10.f;
    float timeUntilLaserDuration = DEFAULT_TIME_UNTIL_LASER;
    float timeUntilLaserElapsed = timeUntilLaserDuration;
    float laserActiveTimeElapsed = 0.f;
    std::default_random_engine timeUntilLaserRandEngine {std::random_device{}()};
    std::default_random_engine laserActiveTimeRandEngine {std::random_device{}()};
    std::uniform_real_distribution<float> timeUntilLaserDistribution {5.f, 15.f};
    std::uniform_real_distribution<float> laserActiveTimeDistribution {3.f, 10.f};

    void reset();
    bool areLasersActive() const;
    void triggerEnteringAnimations();
    void triggerLeavingAnimations();
    void updateSpawnTimers();
    void updateDurationTimer();
    void updateMovement();
    void update();
    void draw() const;
};



#endif //LASERSBEAMS_H
