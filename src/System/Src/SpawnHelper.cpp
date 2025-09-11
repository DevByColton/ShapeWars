#include "../Include/SpawnHelper.h"
#include "../../Entities/Include/PlayerShip.h"
#include "../Include/Extensions.h"

/*
 * Create a new position and make sure it is not within a radius of the player to avoid spawning on top of them
 */
sf::Vector2f SpawnHelper::createSpawnPosition()
{
    sf::Vector2f position;

    do {
        position = {xPositionDistribution(randEngine), yPositionDistribution(randEngine)};
    } while (Extensions::distanceSquared(position, PlayerShip::instance().getPosition()) < positionOffset * positionOffset);

    return position;
}
