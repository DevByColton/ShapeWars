#include "../Include/PauseMenu.h"
#include "../../GameRoot.h"
#include "../../Content/Include/Sound.h"
#include "../../Core/Include/Extensions.h"
#include "../../Entities/Include/Player/PlayerStatus.h"
#include "../../Input/Include/Input.h"
#include "../../Input/Include/MouseAndKeyboard.h"
#include "../Include/GamePlay.h"
#include "../Include/OptionsMenu.h"


PauseMenu::PauseMenu()
{
    // Background and title
    constexpr sf::Color corners = {21, 13, 38, 255};
    constexpr sf::Color middle = {35, 21, 64, 255};
    background.setVertexData(0, {0.0f, 0.0f}, corners);
    background.setVertexData(1, {0, GameRoot::instance().windowSizeF.y}, middle);
    background.setVertexData(2, {GameRoot::instance().windowSizeF.x, 0.0f}, middle);
    background.setVertexData(3, {GameRoot::instance().windowSizeF.x, GameRoot::instance().windowSizeF.y}, corners);

    backgroundTexture.setSmooth(true);
    paused.setOrigin(paused.getLocalBounds().getCenter());
    paused.setPosition({
        GameRoot::instance().windowSizeF.x - 300.f,
        GameRoot::instance().windowSizeF.y / 2.f
    });

    highscoreArea.startOnScreen();

    // Menu options
    options.setOrigin({0.f, options.getLocalBounds().getCenter().y});
    options.setPosition({70.f, backgroundTexture.getSize().y / 2.f});
    options.onSelect = [] { OptionsMenu::instance().open(&instance()); };

    restart.setOrigin({0.f, resume.getLocalBounds().getCenter().y}); // restart another case where alignment is off because of tall letters
    restart.setPosition({70.f, options.getPosition().y - 60.f});
    restart.onSelect = [this]
    {
        PlayerStatus::instance().removeLife(true);
        GamePlay::instance().restartRound();
        resumeGameplay();
    };

    resume.setOrigin({0.f, resume.getLocalBounds().getCenter().y});
    resume.setPosition({70.f, restart.getPosition().y - 60.f});
    resume.onSelect = [this] { resumeGameplay(); };

    quitToMenu.setOrigin({0.f, quitToMenu.getLocalBounds().getCenter().y});
    quitToMenu.setPosition({70.f, options.getPosition().y + 60.f});
    quitToMenu.onSelect = [this]
    {
        PlayerStatus::instance().removeLife(true);
        resumeGameplay();
    };

    quitToDesktop.setOrigin({0.f, quitToDesktop.getLocalBounds().getCenter().y});
    quitToDesktop.setPosition({70.f, quitToMenu.getPosition().y + 60.f});
    quitToDesktop.onSelect = [] { GameRoot::instance().renderWindow.close(); };

    // Set the default active menu option
    setActiveMenuOption(&resume, false);
}


void PauseMenu::processMouseMoved(const sf::Event::MouseMoved* mouseMoved)
{
    // For hovering on an option
    if (!optionIndicator.isActiveOptionIndicatorTransitioning)
        for (int mo = 0; mo < MENU_OPTIONS_COUNT; mo++)
        {
            const bool contains = backgroundSprite
            .getTransform()
            .transformRect(menuOptionPtrs.at(mo)->getGlobalBounds())
            .contains(MouseAndKeyboard::instance().getMouseWindowPosition());

            if (!contains)
                continue;

            // Menu option can be active
            optionIndicator.setActive(
                {menuOptionPtrs.at(mo)->getPosition().x - 30.f, menuOptionPtrs.at(mo)->getPosition().y},
                {menuOptionPtrs.at(mo)->getLocalBounds().size.x + 110.f, menuOptionPtrs.at(mo)->getPosition().y}
            );

            activeMenuOptionIndex = mo;
            setActiveMenuOption(menuOptionPtrs.at(mo), true);
        }
}


void PauseMenu::processMouseReleased(const sf::Event::MouseButtonReleased* mouseReleased)
{
    if (mouseReleased->button == sf::Mouse::Button::Left)
    {
        const bool contains = backgroundSprite
            .getTransform()
            .transformRect(activeMenuOption->getGlobalBounds())
            .contains(MouseAndKeyboard::instance().getMouseWindowPosition());

        if (contains)
            activeMenuOption->onSelect();
    }
}


void PauseMenu::processMouseWheelScrolledEvent(const sf::Event::MouseWheelScrolled* mouseWheelScrolled)
{
    moveToNextMenuOption(-mouseWheelScrolled->delta);
}


void PauseMenu::processKeyPressed(const sf::Event::KeyPressed* keyPressed)
{
    // Nothing to do
}


void PauseMenu::processKeyReleased(const sf::Event::KeyReleased* keyReleased)
{
    if (keyReleased->scancode == sf::Keyboard::Scancode::Escape ||
        keyReleased->scancode == sf::Keyboard::Scancode::Tab)
        resumeGameplay();

    if (keyReleased->scancode == sf::Keyboard::Scancode::Enter ||
        keyReleased->scancode == sf::Keyboard::Scancode::NumpadEnter ||
        keyReleased->scancode == sf::Keyboard::Scancode::Space)
        activeMenuOption->onSelect();

    if (keyReleased->scancode == sf::Keyboard::Scancode::Up)
        moveToNextMenuOption(-1.f);

    if (keyReleased->scancode == sf::Keyboard::Scancode::Down)
        moveToNextMenuOption(1.f);
}


void PauseMenu::processMousePressed(const sf::Event::MouseButtonPressed* mousePressed)
{
    // Nothinig to do
}


void PauseMenu::processJoystickButtonReleased(const sf::Event::JoystickButtonReleased* joystickButtonReleased)
{
    if (Input::isSecondaryButton(joystickButtonReleased))
        resumeGameplay();

    if (Input::isPrimaryButton(joystickButtonReleased))
        activeMenuOption->onSelect();
}


void PauseMenu::processJoystickAxisMoved(const sf::Event::JoystickMoved* joystickMoved)
{
    if (Input::isDpadY(joystickMoved) && Input::wasDpadMoved(joystickMoved))
        moveToNextMenuOption(-joystickMoved->position);

    if (Input::isLeftThumbstickY(joystickMoved) && Input:: wasLeftThumbstickMoved(joystickMoved))
        moveToNextMenuOption(joystickMoved->position);
}


void PauseMenu::update()
{
    optionIndicator.update();
}


void PauseMenu::renderGaussianBlur()
{
    // Nothing to do
}


void PauseMenu::renderToScreen()
{
    backgroundTexture.clear(sf::Color::Transparent);
    background.draw(backgroundTexture);
    backgroundTexture.draw(paused);
    highscoreArea.drawToTexture(backgroundTexture);
    optionIndicator.draw(backgroundTexture);
    backgroundTexture.draw(resume);
    backgroundTexture.draw(restart);
    backgroundTexture.draw(options);
    backgroundTexture.draw(quitToMenu);
    backgroundTexture.draw(quitToDesktop);
    backgroundTexture.display();
    GameRoot::instance().renderWindow.draw(backgroundSprite);
}


void PauseMenu::open()
{
    GameRoot::instance().addUpdatableState(&instance());
    GameRoot::instance().addDrawableState(&instance());
    GameRoot::instance().setActiveInputState(&instance());
    PlayerStatus::instance().stopRoundClock();
}


void PauseMenu::close()
{
    GameRoot::instance().removeUpdatableState(&instance());
    GameRoot::instance().removeDrawableState(&instance());
    setActiveMenuOption(&resume, false);
    activeMenuOptionIndex = 0;
}


void PauseMenu::resumeGameplay()
{
    close();
    GameRoot::instance().addUpdatableState(&GamePlay::instance());
    GameRoot::instance().setActiveInputState(&GamePlay::instance());
    PlayerStatus::instance().startRoundClock();
    Sound::instance().gameplaySong.unmuffle();
}


void PauseMenu::setActiveMenuOption(MenuOption* nextMenuOption, const bool withPositionTransition)
{
    activeMenuOption->setFillColor(MenuOption::MUTED_TEXT_COLOR);
    activeMenuOption = nextMenuOption;
    activeMenuOption->setFillColor(sf::Color::White);

    if (withPositionTransition)
    {
        optionIndicator.setActive(
            {activeMenuOption->getPosition().x - 30.f, activeMenuOption->getPosition().y},
            {activeMenuOption->getLocalBounds().size.x + 110.f, activeMenuOption->getPosition().y}
        );
    }
    else
    {
        optionIndicator.left.setPosition({
            activeMenuOption->getPosition().x - 30.f,
            activeMenuOption->getPosition().y
        });
        optionIndicator.right.setPosition({
            activeMenuOption->getLocalBounds().size.x + 110.f,
            activeMenuOption->getPosition().y
        });
    }
}


void PauseMenu::moveToNextMenuOption(const float direction)
{
    if (optionIndicator.isActiveOptionIndicatorTransitioning)
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

    setActiveMenuOption(menuOptionPtrs.at(activeMenuOptionIndex), true);
}
