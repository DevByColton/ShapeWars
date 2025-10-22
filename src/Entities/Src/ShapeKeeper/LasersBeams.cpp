#include "../../Include/ShapeKeeper/LasersBeams.h"
#include "../../../Core/Include/RandomVector.h"
#include "../../../Systems/Include/Particles.h"


LasersBeams::Laser::Laser(const sf::Vector2f& position)
{
    laser.setOrigin({
        laser.getTexture().getSize().x / 2.f,
        laser.getTexture().getSize().y / 2.f
    });
    laser.setPosition(position);
    laser.setRotation(sf::Angle::Zero);

    // Set up the colliders
    colliders.fill(sf::CircleShape(15.f));
    for (int colliderIndex = 0; colliderIndex < COLLIDERS_SIZE; colliderIndex++)
    {
        colliders.at(colliderIndex).setPosition(position);
        colliders.at(colliderIndex).setOrigin(colliders.at(colliderIndex).getGeometricCenter());
        colliders.at(colliderIndex).setFillColor(sf::Color::Red);
    }
}


LasersBeams::LasersBeams(ShapeKeeperCore* core)
{
    this->core = core;
    lasers.fill(Laser(core->getPosition()));
}


void LasersBeams::reset()
{
    laserRotation = sf::Angle::Zero;
    timeUntilLaserDuration = DEFAULT_TIME_UNTIL_LASER;
    timeUntilLaserElapsed = 0.f;
    laserActiveTimeElapsed = 0.f;

    for (int i = 0; i < LASERS_SIZE; i++)
        lasers.at(i).reset();
}


void LasersBeams::Laser::reset()
{
    laserExpandTime = 0.f;
    laserCollapseTime = LASER_COLLAPSE_DURATION;
    animationState = Idle;
    laser.setScale({1.f, 1.f});
}


void LasersBeams::Laser::updateAnimation(const sf::Vector2f& laserEndPoint)
{
    if (animationState == Idle)
        return;

    // Entering animation state
    if (animationState == Entering && laserExpandTime < LASER_EXPAND_DURATION)
    {
        laserExpandTime += GameRoot::instance().deltaTime;

        // Casts both x and y to ints
        int x = laser.getTexture().getSize().x;
        const int y = laser.getTexture().getSize().y;

        if (laserExpandTime > LASER_EXPAND_DURATION)
        {
            animationState = Active;
            laserExpandTime = 0.f;
        }
        else
        {
            x *= laserExpandTime / LASER_EXPAND_DURATION;
        }

        // This draws only portion of the texture to give it the illusion of beaming to a full length
        laser.setTextureRect({{0, 0},{x, y}});

        return;
    }

    // Active animation state
    if (animationState == Active)
    {
        laser.setScale({
            1.f,
            0.15f * std::sin(60.f * GameRoot::instance().elapsedGameTime) + 0.85f
        });

        // Create some particles at the blast location
        for (int p = 0; p < 10; p++)
            Particles::instance().create(
                1.25f,
                DontIgnoreGravity,
                p % 2 == 0 ? Explosion : Spark,
                laser.getPosition() + laserEndPoint,
                RandomVector::instance().next(3.f, 4.f),
                blastColor
            );

        return;
    }

    // Leaving animation state
    if (animationState == Leaving && laserCollapseTime > 0.f)
    {
        laserCollapseTime -= GameRoot::instance().deltaTime;

        if (laserCollapseTime < 0.f)
        {
            animationState = Idle;
            laserCollapseTime = LASER_COLLAPSE_DURATION;
            laser.setScale({1.f, 1.f});
        }
        else
        {
            laser.setScale({1.f, laserCollapseTime / LASER_COLLAPSE_DURATION});
        }
    }
}


bool LasersBeams::areLasersActive() const
{
    bool isAtLeastOneActive = false;

    for (int i = 0; i < LASERS_SIZE; i++)
        if (lasers.at(i).animationState != Idle)
        {
            isAtLeastOneActive = true;
            break;
        }

    return isAtLeastOneActive;
}


void LasersBeams::triggerEnteringAnimations()
{
    for (int i = 0; i < LASERS_SIZE; i++)
        lasers.at(i).animationState = Entering;
}


void LasersBeams::triggerLeavingAnimations()
{
    for (int i = 0; i < LASERS_SIZE; i++)
        lasers.at(i).animationState = Leaving;
}


void LasersBeams::updateSpawnTimers()
{
    if (areLasersActive())
        return;

    if (timeUntilLaserElapsed < timeUntilLaserDuration)
    {
        timeUntilLaserElapsed += GameRoot::instance().deltaTime;

        // Trigger laser
        if (timeUntilLaserElapsed > timeUntilLaserDuration)
        {
            timeUntilLaserElapsed = 0.f;
            timeUntilLaserDuration = timeUntilLaserDistribution(timeUntilLaserRandEngine);

            laserActiveTimeElapsed = laserActiveTimeDistribution(laserActiveTimeRandEngine);
            triggerEnteringAnimations();
        }
    }
}


void LasersBeams::updateDurationTimer()
{
    if (!areLasersActive())
        return;

    laserActiveTimeElapsed -= GameRoot::instance().deltaTime;

    if (laserActiveTimeElapsed < 0.f)
        triggerLeavingAnimations();
}


void LasersBeams::updateMovement()
{
    // Update the base laser rotation
    laserRotation += sf::radians(GameRoot::instance().deltaTime * 0.25f);

    // Laser angle for each rotation starting at the base rotation
    float laserAngle = laserRotation.asRadians();
    for (int i = 0; i < LASERS_SIZE; i++)
    {
        const Quaternion quaternion = Quaternion::createFromYawPitchRoll(0, 0, laserAngle);
        const sf::Vector2f offset = Extensions::transform({lasers.at(0).laser.getTexture().getSize().x / 2.f, 0.f}, quaternion);
        const sf::Vector2f laserEndPoint = Extensions::transform({lasers.at(0).laser.getTexture().getSize().x / 2.f - 17.5f, 0.f}, quaternion);

        lasers.at(i).laser.setPosition(core->getPosition() + offset);
        lasers.at(i).laser.setRotation(sf::radians(Extensions::toAngle(laserEndPoint)));
        lasers.at(i).updateAnimation(laserEndPoint);

        // Rotate the laser angle for the next laser
        laserAngle += sf::radians(TWO_PI / LASERS_SIZE).asRadians();

        // Update colliders
        float colliderOffset = 27.5f;
        for (int colliderIndex = 0; colliderIndex < Laser::COLLIDERS_SIZE; colliderIndex++)
        {
            lasers.at(i).colliders.at(colliderIndex).setPosition(lasers.at(i).laser.getPosition() + Extensions::transform({-(lasers.at(0).laser.getTexture().getSize().x / 2.f) + colliderOffset, 0.f}, quaternion));
            colliderOffset += lasers.at(i).colliders.at(colliderIndex).getRadius() * 2.f;
        }
    }
}


void LasersBeams::update()
{
    updateSpawnTimers();
    updateMovement();
    updateDurationTimer();
}


void LasersBeams::draw() const
{
    if (!areLasersActive())
        return;

    for (int i = 0; i < LASERS_SIZE; ++i)
    {
        GaussianBlur::instance().drawToBase(lasers.at(i).laser);

        // For debugging
        // for (int colliderIndex = 0; colliderIndex < Laser::COLLIDERS_SIZE; colliderIndex++)
        //     GaussianBlur::instance().drawToBase(lasers.at(i).colliders.at(colliderIndex));
    }
}
