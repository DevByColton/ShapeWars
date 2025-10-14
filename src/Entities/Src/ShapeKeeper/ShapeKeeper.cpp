#include "../../Include/ShapeKeeper/ShapeKeeper.h"


void ShapeKeeper::startEncounter()
{
    // Trigger health container transitions
    if (!healthContainer.isTransitioningIn && !healthContainer.isTransitioningOut)
        healthContainer.isTransitioningIn = true;

    // Trigger boss enter
}


void ShapeKeeper::endEncounter()
{
    // Trigger health container transitions
    if (!healthContainer.isTransitioningOut && !healthContainer.isTransitioningIn)
        healthContainer.isTransitioningOut = true;

    // Trigger boss leave or resets??
}


bool ShapeKeeper::canTakeCoreDamage() const
{
    return !top.isAlive() && !middleLeft.isAlive() && !middleRight.isAlive() && !bottomLeft.isAlive() && !bottomRight.isAlive();
}


void ShapeKeeper::update()
{
    healthContainer.transitionIn();
    healthContainer.transitionOut();

    // Update each part first
    core.update(); // Todo: base core movement
    top.update();
    middleLeft.update();
    middleRight.update();
    bottomLeft.update();
    bottomRight.update();

    // TODO: Periodically set enemies spawn to true for a few seconds or so

    // TODO: Check if an attack type is running, if not activate new one chance

    // Update each special attack type
    directionalAttack.update(&top);
    directionalAttack.update(&middleLeft);
    directionalAttack.update(&middleRight);
    directionalAttack.update(&bottomLeft);
    directionalAttack.update(&bottomRight);

    lasersAttack.update(&top);
    lasersAttack.update(&middleLeft);
    lasersAttack.update(&middleRight);
    lasersAttack.update(&bottomLeft);
    lasersAttack.update(&bottomRight);
}


void ShapeKeeper::draw()
{
    top.draw();
    middleLeft.draw();
    middleRight.draw();
    bottomLeft.draw();
    bottomRight.draw();
    core.draw(canTakeCoreDamage());
    healthContainer.draw();
}


void ShapeKeeper::drawText()
{
    healthContainer.drawText();
}

