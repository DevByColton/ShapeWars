#include "../Include/PauseMenu.h"
#include "../../GameRoot.h"
#include "../../Core/Include/Extensions.h"
#include "../../Entities/Include/Player/PlayerStatus.h"
#include "../../Input/Include/Input.h"
#include "../Include/GamePlay.h"


PauseMenu::PauseMenu()
{
    // Justify the pause text slightly under the multiplier text in the middle of the screen
    pausedText.setPosition({GameRoot::instance().windowSizeF.x / 2.f, 250.f});
    pausedText.setOrigin({ pausedText.getLocalBounds().size.x / 2.f, pausedText.getLocalBounds().size.y / 2.f });

    // Set the highscore text to be in the top right of the window, right aligned
    highScoreHeaderText.setOrigin({ highScoreHeaderText.getLocalBounds().size.x, highScoreHeaderText.getLocalBounds().size.y});
    highScoreHeaderText.setPosition({ GameRoot::instance().windowSizeF.x - 22.f, 25.f });
    highScoreHeaderText.setStyle(sf::Text::Bold);
    highScoreText.setStyle(sf::Text::Bold);
}


void PauseMenu::processMouseReleased(const sf::Event::MouseButtonReleased* mouseReleased)
{
    // nothing yet
}

void PauseMenu::processKeyReleased(const sf::Event::KeyReleased* keyReleased)
{
    if (keyReleased->scancode == sf::Keyboard::Scancode::P)
        resume();
}

void PauseMenu::processJoystickButtonReleased(const sf::Event::JoystickButtonReleased* joystickButtonReleased)
{
    if (Input::isStartButton(joystickButtonReleased))
        resume();
}

void PauseMenu::processJoystickAxisMoved(const sf::Event::JoystickMoved* joystickMoved)
{
}

void PauseMenu::update()
{
}


void PauseMenu::renderGaussianBlur()
{
}


void PauseMenu::renderToScreen()
{
    GameRoot::instance().renderWindow.draw(pausedText);

    // Draw the high score header and text, right align the highscore number
    GameRoot::instance().renderWindow.draw(highScoreHeaderText);
    highScoreText.setString(Extensions::formatNumberWithCommas(PlayerStatus::instance().highScore));
    const sf::FloatRect highscoreTextRect = highScoreText.getLocalBounds();
    highScoreText.setPosition({ GameRoot::instance().windowSizeF.x - highscoreTextRect.size.x - 22.f, 30.f });
    GameRoot::instance().renderWindow.draw(highScoreText);
}

void PauseMenu::resume()
{
    GameRoot::instance().removeUpdatableState(&instance());
    GameRoot::instance().removeDrawableState(&instance());
    GameRoot::instance().addUpdatableState(&GamePlay::instance());
    GameRoot::instance().setActiveInputState(&GamePlay::instance());
    PlayerStatus::instance().startRoundClock();
}
