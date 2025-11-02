#include "../Include/StartMenu.h"
#include "../../Content/Include/GaussianBlur.h"
#include "../../Core/Include/ColorPicker.h"
#include "../../Core/Include/Extensions.h"
#include "../../Core/Include/Logger.h"
#include "../../Core/Include/RandomVector.h"
#include "../../Input/Include/Input.h"
#include "../../Input/Include/MouseAndKeyboard.h"
#include "../../Systems/Include/Grid.h"
#include "../../Systems/Include/Particles.h"
#include "../Include/GamePlay.h"


StartMenu::StartMenu()
{
    // Menu options
    menuOptionsSprite.setOrigin(menuOptionsSprite.getLocalBounds().getCenter());
    menuOptionsSprite.setPosition(menuOptionsOnScreenPosition);
    start.setOrigin({0, start.getLocalBounds().getCenter().y});
    start.setPosition({70.f, menuOptionsTexture.getSize().y * 0.25f});
    options.setOrigin({0, options.getLocalBounds().getCenter().y});
    options.setPosition({70.f, menuOptionsTexture.getSize().y * 0.5f});
    quit.setOrigin({0, quit.getLocalBounds().getCenter().y});
    quit.setPosition({70.f, menuOptionsTexture.getSize().y * 0.75f});
    quit.onSelect = [] { GameRoot::instance().renderWindow.close(); };
    setActiveMenuOption(&start);

    // Select functions
    start.onSelect = [this]{ isTransitioningOut = true; };
    options.onSelect = [] { Logger::printOut("todo: going to options screen"); };
    quit.onSelect = [] { GameRoot::instance().renderWindow.close(); };

    // Set the default active menu option
    leftIndicator.setPosition({
        start.getPosition().x - 30.f,
        start.getPosition().y
    });
    rightIndicator.setPosition({
        start.getLocalBounds().size.x + 110.f,
        start.getPosition().y
    });

    title.setOrigin(title.getLocalBounds().getCenter());
    title.setPosition(titleOnScreenPosition);

    shapeText.setOrigin(shapeText.getLocalBounds().getCenter());
    shapeText.setPosition({title.getOrigin().x, title.getOrigin().y - 60.f});

    warsText.setOrigin({warsText.getLocalBounds().size.x, warsText.getLocalBounds().getCenter().y});
    warsText.setPosition({title.getLocalBounds().size.x - 55.f, title.getOrigin().y + 60.f});
}


void StartMenu::processMouseReleased(const sf::Event::MouseButtonReleased* mouseReleased)
{
    if (mouseReleased->button == sf::Mouse::Button::Left)
    {
        const bool contains = menuOptionsSprite
            .getTransform()
            .transformRect(activeMenuOption->getGlobalBounds())
            .contains(MouseAndKeyboard::instance().getMouseWindowPosition());

        if (contains)
            activeMenuOption->onSelect();
    }
}

void StartMenu::processKeyReleased(const sf::Event::KeyReleased* keyReleased)
{
    if (keyReleased->scancode == sf::Keyboard::Scancode::Escape)
        GameRoot::instance().renderWindow.close();

    if (keyReleased->scancode == sf::Keyboard::Scancode::Enter ||
        keyReleased->scancode == sf::Keyboard::Scancode::NumpadEnter ||
        keyReleased->scancode == sf::Keyboard::Scancode::Space)
        activeMenuOption->onSelect();

    if (keyReleased->scancode == sf::Keyboard::Scancode::Up)
        moveToNextMenuOption(-1.f);

    if (keyReleased->scancode == sf::Keyboard::Scancode::Down)
        moveToNextMenuOption(1.f);
}

void StartMenu::processJoystickButtonReleased(const sf::Event::JoystickButtonReleased* joystickButtonReleased)
{
    if (Input::isPrimaryButton(joystickButtonReleased))
        activeMenuOption->onSelect();

    if (Input::isStartButton(joystickButtonReleased))
        options.onSelect();
}

void StartMenu::processJoystickAxisMoved(const sf::Event::JoystickMoved* joystickMoved)
{
    if (Input::isDpadY(joystickMoved) && Input::wasDpadMoved(joystickMoved))
        moveToNextMenuOption(-joystickMoved->position);

    if (Input::isLeftThumbstickY(joystickMoved) && Input:: wasLeftThumbstickMoved(joystickMoved))
        moveToNextMenuOption(joystickMoved->position);
}


void StartMenu::moveToNextMenuOption(const float direction)
{
    if (isActiveOptionIndicatorTransitioning)
        return;

    // Iterate the active index
    if (direction < 0)
    {
        activeMenuOptionIndex -= 1;

        if (activeMenuOptionIndex < 0)
            activeMenuOptionIndex = MENU_OPTIONS_COUNT - 1;
    }
    else
    {
        activeMenuOptionIndex += 1;

        if (activeMenuOptionIndex > MENU_OPTIONS_COUNT - 1)
            activeMenuOptionIndex = 0;
    }

    // Set the next active menu option
    setActiveMenuOption(menuOptionPtrs.at(activeMenuOptionIndex));

    leftIndicator.setActive({
        activeMenuOption->getPosition().x - 30.f,
        activeMenuOption->getPosition().y
    });
    rightIndicator.setActive({
        activeMenuOption->getLocalBounds().size.x + 110.f,
        activeMenuOption->getPosition().y
    });

    isActiveOptionIndicatorTransitioning = true;
}


void StartMenu::update()
{
    updateBackground();
    updateMenuOptions();

    transitionMenuAndTitleIn();
    if (transitionMenuAndTitleOut())
    {
        GameRoot::instance().removeUpdatableState(&instance());
        GameRoot::instance().removeDrawableState(&instance());
        GameRoot::instance().addUpdatableState(&GamePlay::instance());
        GameRoot::instance().addDrawableState(&GamePlay::instance());
        GameRoot::instance().setActiveInputState(&GamePlay::instance());
        GamePlay::instance().startRound();
    }
}


void StartMenu::updateBackground()
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


void StartMenu::updateMenuOptions()
{
    // For hovering on an option
    if (!isActiveOptionIndicatorTransitioning)
        for (int mo = 0; mo < MENU_OPTIONS_COUNT; mo++)
        {
            const bool contains = menuOptionsSprite
            .getTransform()
            .transformRect(menuOptionPtrs.at(mo)->getGlobalBounds())
            .contains(MouseAndKeyboard::instance().getMouseWindowPosition());

            if (!contains)
                continue;

            // Menu option can be active
            leftIndicator.setActive({
                menuOptionPtrs.at(mo)->getPosition().x - 30.f,
                menuOptionPtrs.at(mo)->getPosition().y
            });
            rightIndicator.setActive({
                menuOptionPtrs.at(mo)->getLocalBounds().size.x + 110.f,
                menuOptionPtrs.at(mo)->getPosition().y
            });

            activeMenuOptionIndex = mo;
            setActiveMenuOption(menuOptionPtrs.at(mo));
            isActiveOptionIndicatorTransitioning = true;
        }

    if (isActiveOptionIndicatorTransitioning)
    {
        indicatorsTransitionTime += GameRoot::instance().deltaTime;

        const float time = indicatorsTransitionTime / INDICATORS_TRANSITION_DURATION;
        leftIndicator.transition(time);
        rightIndicator.transition(time);

        // Set stopping point
        if (indicatorsTransitionTime > INDICATORS_TRANSITION_DURATION)
        {
            isActiveOptionIndicatorTransitioning = false;
            indicatorsTransitionTime = 0.f;
            leftIndicator.setPosition(leftIndicator.targetPosition);
            rightIndicator.setPosition(rightIndicator.targetPosition);
        }
    }

    leftIndicator.rotate(sf::radians(2.f * GameRoot::instance().deltaTime));
    rightIndicator.rotate(sf::radians(-2.f * GameRoot::instance().deltaTime));
}


void StartMenu::ActiveMenuOptionIndicator::setActive(const sf::Vector2f& targetPosition)
{
    previousPosition = getPosition();
    this->targetPosition = targetPosition;
}


void StartMenu::setActiveMenuOption(MenuOption* nextMenuOption)
{
    activeMenuOption->setFillColor(MenuOption::MUTED_TEXT_COLOR);
    activeMenuOption = nextMenuOption;
    activeMenuOption->setFillColor(sf::Color::White);
}


void StartMenu::ActiveMenuOptionIndicator::transition(const float time)
{
    setPosition(Extensions::easeOutCubic(previousPosition, targetPosition, time));
}


void StartMenu::transitionMenuAndTitleIn()
{
    if (isTransitioningIn)
    {
        // Increment for position ease
        transitionTime += GameRoot::instance().deltaTime;

        // Ease position
        menuOptionsSprite.setPosition(Extensions::easeOutCubic(menuOptionsOffScreenPosition, menuOptionsOnScreenPosition, transitionTime / TRANSITION_DURATION));
        title.setPosition(Extensions::easeOutCubic(titleOffScreenPosition, titleOnScreenPosition, transitionTime / TRANSITION_DURATION));

        // Set stopping point
        if (transitionTime > TRANSITION_DURATION)
        {
            isTransitioningIn = false;
            transitionTime = 0.f;
            menuOptionsSprite.setPosition(menuOptionsOnScreenPosition);
            title.setPosition(titleOnScreenPosition);
        }
    }
}


bool StartMenu::transitionMenuAndTitleOut()
{
    if (isTransitioningOut)
    {
        // Increment for position ease
        transitionTime += GameRoot::instance().deltaTime;

        // Ease position
        menuOptionsSprite.setPosition(Extensions::easeInCubic(menuOptionsOnScreenPosition, menuOptionsOffScreenPosition, transitionTime / TRANSITION_DURATION));
        title.setPosition(Extensions::easeInCubic(titleOnScreenPosition, titleOffScreenPosition, transitionTime / TRANSITION_DURATION));

        // Set stopping point
        if (transitionTime > TRANSITION_DURATION)
        {
            isTransitioningOut = false;
            transitionTime = 0.f;
            menuOptionsSprite.setPosition(menuOptionsOffScreenPosition);
            title.setPosition(titleOffScreenPosition);
            return true;
        }
    }

    return false;
}


void StartMenu::renderGaussianBlur()
{
    GaussianBlur::instance().clearTextures();
    Particles::instance().draw();
    Grid::instance().draw();
    menuOptionsTexture.clear(sf::Color::Transparent);
    menuOptionsTexture.draw(leftIndicator);
    menuOptionsTexture.draw(rightIndicator);
    menuOptionsTexture.display();
    GaussianBlur::instance().drawToBase(menuOptionsSprite);
    GaussianBlur::instance().drawToScreen();
}


void StartMenu::renderToScreen()
{
    menuOptionsTexture.clear(sf::Color::Transparent);
    menuOptionsTexture.draw(start);
    menuOptionsTexture.draw(options);
    menuOptionsTexture.draw(quit);
    menuOptionsTexture.display();
    GameRoot::instance().renderWindow.draw(menuOptionsSprite);

    titleTexture.clear(sf::Color::Transparent);
    titleTexture.draw(shapeText);
    titleTexture.draw(warsText);
    titleTexture.display();
    GameRoot::instance().renderWindow.draw(title);
}
