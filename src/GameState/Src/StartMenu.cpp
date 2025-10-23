#include "../Include/StartMenu.h"
#include "../../Content/Include/GaussianBlur.h"
#include "../../Systems/Include/Grid.h"
#include "../../Systems/Include/Particles.h"


void StartMenu::processKeyReleased(const sf::Event::KeyReleased* keyReleased)
{
    // Todo;
    if (keyReleased->scancode == sf::Keyboard::Scancode::Num1)
        GameRoot::instance().setCurrentGameState(InGamePlay);
}

void StartMenu::processJoystickButtonReleased(const sf::Event::JoystickButtonReleased* joystickButtonReleased)
{
    // Todo;
}

void StartMenu::processJoystickAxisMoved(const sf::Event::JoystickMoved* joystickMoved) {}


void StartMenu::update()
{
    Particles::instance().update();
    Grid::instance().update();
}

void StartMenu::renderGaussianBlur()
{
    GaussianBlur::instance().clearTextures();
    Particles::instance().draw();
    Grid::instance().draw();
    GaussianBlur::instance().drawToScreen();
}


void StartMenu::renderToScreen()
{

}
