#include "../Include/StartMenu.h"
#include "../../Content/Include/GaussianBlur.h"
#include "../../Core/Include/ColorPicker.h"
#include "../../Core/Include/RandomVector.h"
#include "../../Systems/Include/Grid.h"
#include "../../Systems/Include/Particles.h"
#include "../Include/GamePlay.h"


void StartMenu::processKeyReleased(const sf::Event::KeyReleased* keyReleased)
{
    // Todo;
    if (keyReleased->scancode == sf::Keyboard::Scancode::Num1)
    {
        GameRoot::instance().setCurrentGameState(InGamePlay);
        GamePlay::instance().startRound();
    }
}

void StartMenu::processJoystickButtonReleased(const sf::Event::JoystickButtonReleased* joystickButtonReleased)
{
    // Todo;
}

void StartMenu::processJoystickAxisMoved(const sf::Event::JoystickMoved* joystickMoved) {}


void StartMenu::update()
{
    timeUntilNewExplosionElapsed -= GameRoot::instance().deltaTime;
    if (timeUntilNewExplosionElapsed < 0.f)
    {
        timeUntilNewExplosionElapsed = TIME_UNTIL_NEW_EXPLOSION_DURATION;

        // Spawn a particle explosion
        const float hue1 = ColorPicker::instance().generateHue();
        const float hue2 = ColorPicker::instance().generateShiftedHue(hue1);
        const sf::Color color1 = ColorPicker::instance().hsvToRgb(hue1, 0.9f, 0.8f);
        const sf::Color color2 = ColorPicker::instance().hsvToRgb(hue2, 0.9f, 0.8f);
        const sf::Vector2f position = {widthDistribution(widthRandEngine),heightDistribution(heightRandEngine)};
        const float particleDuration = particleDurationDistribution(particleDurationRandEngine);
        for (int i = 0; i < 120; i++)
        {
            Particles::instance().create(
                particleDuration,
                IgnoreGravity,
                Spark,
                position,
                RandomVector::instance().next(2.f, 18.f),
                ColorPicker::instance().lerp(color1, color2)
            );
        }
    }

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
