#include "../../Include/ShapeKeeper/ShapeKeeper.h"
#include "../../Include/Player/PlayerShip.h"


void ShapeKeeper::startEncounter()
{
    if (isActive)
        return;

    // Trigger health container transitions
    if (!healthContainer.isTransitioningIn)
        healthContainer.isTransitioningIn = true;

    // Spawn the boss opposite of the player, by default the boss is on the left side
    // Make sure all parts are in sync
    core.activate(PlayerShip::instance().getPosition().x > GameRoot::instance().windowSizeF.x / 2.f);
    core.onDeath = [this]{ endEncounter(); };
    top.reset();
    middleLeft.reset();
    middleRight.reset();
    bottomLeft.reset();
    bottomRight.reset();

    isActive = true;
}


void ShapeKeeper::endEncounter()
{
    if (!isActive)
        return;

    // Trigger health container transitions
    if (!healthContainer.isTransitioningOut)
        healthContainer.isTransitioningOut = true;

    isActive = false;
}


bool ShapeKeeper::canTakeCoreDamage() const
{
    return !top.isAlive() && !middleLeft.isAlive() && !middleRight.isAlive() && !bottomLeft.isAlive() && !bottomRight.isAlive();
}


void ShapeKeeper::update()
{
    healthContainer.transitionIn();
    healthContainer.transitionOut();

    if (!isActive)
        return;

    // Update each part first
    core.update();
    top.update();
    middleLeft.update();
    middleRight.update();
    bottomLeft.update();
    bottomRight.update();

    // TODO: Periodically set enemies spawn to true for a few seconds or so

    // TODO: Check if an attack type is running, if not activate new one chance

    // Update lasers
    lasersAttack.update(&top);
    lasersAttack.update(&middleLeft);
    lasersAttack.update(&middleRight);
    lasersAttack.update(&bottomLeft);
    lasersAttack.update(&bottomRight);
}


void ShapeKeeper::draw()
{
    healthContainer.draw();

    if (!isActive)
        return;

    top.draw();
    middleLeft.draw();
    middleRight.draw();
    bottomLeft.draw();
    bottomRight.draw();
    core.draw(canTakeCoreDamage());
}


void ShapeKeeper::drawText()
{
    healthContainer.drawText();
}

