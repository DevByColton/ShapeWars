#include "../Include/GamePlayHUD.h"
#include "../../../GameRoot.h"
#include "../../../Core/Include/Extensions.h"
#include "../../../Entities/Include/Player/Buffs.h"
#include "../../../Entities/Include/Player/PlayerStatus.h"


GamePlayHUD::GamePlayHUD()
{
    // Set the score to be in the top middle of the screen
    scoreAreaTexture.setSmooth(true);
    scoreArea.setPosition(scoreAreaOnScreenPosition);
    scoreArea.setOrigin({
        scoreAreaTexture.getSize().x / 2.f,
        scoreAreaTexture.getSize().y / 2.f
    });

    scoreText.setPosition({scoreAreaTexture.getSize().x / 2.f, 30.f});

    // Set the multiplier text to be in the right top of the window
    multiplierHeaderText.setOrigin({
        multiplierHeaderText.getLocalBounds().size.x / 2.f,
        multiplierHeaderText.getLocalBounds().size.y / 2.f
    });
    multiplierHeaderText.setPosition({ scoreText.getPosition().x + 132.5f, scoreText.getPosition().y + 50.f });
    multiplierHeaderText.setStyle(sf::Text::Bold);
    multiplierText.setPosition({ multiplierHeaderText.getPosition().x + 51.5f, multiplierHeaderText.getPosition().y + 1.f });
    multiplierText.setStyle(sf::Text::Bold);

    // Set the timer text just under the score and multiplier
    timerText.setPosition({ scoreText.getPosition().x - 167.5f, scoreText.getPosition().y + 50.f });
    timerText.setLetterSpacing(1.5f);
    timerText.setStyle(sf::Text::Bold);

    // Objective text
    objectiveText.setStyle(sf::Text::Bold);
    objectiveText.setOrigin({
        objectiveText.getLocalBounds().size.x / 2.f,
        objectiveText.getLocalBounds().size.y / 2.f
    });
    objectiveText.setPosition({
        scoreAreaTexture.getSize().x / 2.f,
        scoreAreaTexture.getSize().y - 38.f
    });

    // Set the game over text in the middle of the screen
    gameOverText.setPosition({GameRoot::instance().windowSizeF.x / 2.f, GameRoot::instance().windowSizeF.y / 2.f});
}


void GamePlayHUD::resetObjective()
{
    objectiveText.setString(SCORE_OBJECTIVE_TEXT);
    objectiveText.setOrigin({
        objectiveText.getLocalBounds().size.x / 2.f,
        objectiveText.getLocalBounds().size.y / 2.f
    });
}


void GamePlayHUD::setObjectiveEndless()
{
    objectiveText.setString(ENDLESS_OBJECTIVE_TEXT);
    objectiveText.setOrigin({
        objectiveText.getLocalBounds().size.x / 2.f,
        objectiveText.getLocalBounds().size.y / 2.f
    });
}


void GamePlayHUD::markHealthAreaTransitionOut(const bool shouldStartScoreArea)
{
    this->shouldStartScoreArea = shouldStartScoreArea;
    healthContainer.isTransitioningOut = true;
}


void GamePlayHUD::markScoreAreaTransitionOut(const bool shouldStartHealthArea)
{
    this->shouldStartHealthArea = shouldStartHealthArea;
    isTransitioningScoreAreaOut = true;
}


bool GamePlayHUD::transitionInScoreArea()
{
    if (isTransitioningScoreAreaIn)
    {
        // Increment for position ease
        transitionTime += GameRoot::instance().deltaTime;

        // Ease position
        scoreArea.setPosition(Extensions::easeOutCubic(scoreAreaOffScreenPosition, scoreAreaOnScreenPosition, transitionTime / TRANSITION_DURATION));

        // Set stopping point
        if (transitionTime > TRANSITION_DURATION)
        {
            isTransitioningScoreAreaIn = false;
            transitionTime = 0.f;
            scoreArea.setPosition(scoreAreaOnScreenPosition);

            return true;
        }
    }

    return false;
}


bool GamePlayHUD::transitionOutScoreArea()
{
    if (isTransitioningScoreAreaOut)
    {
        // Increment for position ease
        transitionTime += GameRoot::instance().deltaTime;

        // Ease position
        scoreArea.setPosition(Extensions::easeInCubic(scoreAreaOnScreenPosition, scoreAreaOffScreenPosition, transitionTime / TRANSITION_DURATION));

        // Set stopping point
        if (transitionTime > TRANSITION_DURATION)
        {
            isTransitioningScoreAreaOut = false;
            transitionTime = 0.f;
            scoreArea.setPosition(scoreAreaOffScreenPosition);
            healthContainer.isTransitioningIn = shouldStartHealthArea;
            shouldStartHealthArea = false;
            return true;
        }
    }

    return false;
}


void GamePlayHUD::update()
{
    // Update any transitions that need to happen
    gamePlayControlArea.transitionIn();
    gamePlayControlArea.transitionOut();
    transitionInScoreArea();
    transitionOutScoreArea();
    healthContainer.transitionIn();

    if (healthContainer.transitionOut())
    {
        isTransitioningScoreAreaIn = shouldStartScoreArea;
        shouldStartScoreArea = false;
    }
}


std::string GamePlayHUD::formattedTime()
{
    const int minutes = static_cast<int>(PlayerStatus::instance().roundTimeSeconds) / 60;
    const int seconds = static_cast<int>(PlayerStatus::instance().roundTimeSeconds) % 60;

    std::string timeString {std::to_string(minutes)};
    timeString.append(":");

    if (seconds < 10)
        timeString.append("0" + std::to_string(seconds));
    else
        timeString.append(std::to_string(seconds));

    return timeString;
}


void GamePlayHUD::drawToBlur()
{
    healthContainer.draw();
    gamePlayControlArea.drawToBlur();
}


void GamePlayHUD::drawToScreen()
{
    // Draw everything to the area texture
    scoreAreaTexture.clear(sf::Color::Transparent);

    // Draw the score frame
    scoreFrame.setColor({255, 255, 255, static_cast<std::uint8_t>(255 * GameRoot::instance().frameUIOpacity)});
    scoreAreaTexture.draw(scoreFrame);

    // Draw the score text and reset the origin based on how large the text value is
    scoreText.setString(Extensions::formatNumberWithCommas(PlayerStatus::instance().score));
    scoreText.setOrigin({
        scoreText.getLocalBounds().size.x / 2.f,
        scoreText.getLocalBounds().size.y / 2.f,
    });
    scoreAreaTexture.draw(scoreText);

    // Draw the multiplier header and text
    multiplierText.setString(std::to_string(PlayerStatus::instance().multiplier));
    multiplierText.setOrigin({
        multiplierText.getLocalBounds().size.x / 2.f,
        multiplierText.getLocalBounds().size.y / 2.f
    });
    scoreAreaTexture.draw(multiplierHeaderText);
    scoreAreaTexture.draw(multiplierText);

    // Draw the timer text
    timerText.setString(formattedTime());
    timerText.setOrigin({timerText.getLocalBounds().size.x / 2.f, timerText.getLocalBounds().size.y / 2.f});
    scoreAreaTexture.draw(timerText);

    // Objective text
    scoreAreaTexture.draw(objectiveText);

    // Display and draw to screen
    scoreAreaTexture.display();
    GameRoot::instance().renderWindow.draw(scoreArea);

    gamePlayControlArea.drawToScreen();
    healthContainer.drawText();

    // Draw game over text
    if (PlayerStatus::instance().isGameOver()) {
        std::string text {"== [ game over ]== \n"};
        text.append("your score: ");
        text.append(Extensions::formatNumberWithCommas(PlayerStatus::instance().score) + "\n");
        text.append("high score: ");
        text.append(Extensions::formatNumberWithCommas(PlayerStatus::instance().highScore));
        gameOverText.setString(text);
        gameOverText.setOrigin({gameOverText.getLocalBounds().size.x / 2.f, gameOverText.getLocalBounds().size.y / 2.f});
        GameRoot::instance().renderWindow.draw(gameOverText);
    }
}
