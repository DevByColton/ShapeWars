#include <string>
#include "../Include/UserInterface.h"
#include "../../GameRoot.h"
#include "../../Content/Include/GaussianBlur.h"
#include "../../Entities/Include/Player/PlayerStatus.h"
#include "../../System/Include/Extensions.h"


UserInterface::UserInterface()
{
    // Set the score to be in the top middle of the screen
    scoreText.setPosition({GameRoot::instance().windowSizeF.x / 2.f, 15.f});
    scoreFrame.setOrigin({
        scoreFrame.getTexture().getSize().x / 2.f,
        scoreFrame.getTexture().getSize().y / 2.f
    });
    scoreFrame.setPosition({
        GameRoot::instance().windowSizeF.x / 2.f,
        42.5
    });

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

    // Justify the pause text slightly under the multiplier text in the middle of the screen
    pausedText.setPosition({GameRoot::instance().windowSizeF.x / 2.f, 250.f});
    pausedText.setOrigin({ pausedText.getLocalBounds().size.x / 2.f, pausedText.getLocalBounds().size.y / 2.f });

    // Set the game over text in the middle of the screen
    gameOverText.setPosition({GameRoot::instance().windowSizeF.x / 2.f, GameRoot::instance().windowSizeF.y / 2.f});

    // Set the highscore text to be in the top right of the window, right aligned
    highScoreHeaderText.setOrigin({ highScoreHeaderText.getLocalBounds().size.x, highScoreHeaderText.getLocalBounds().size.y});
    highScoreHeaderText.setPosition({ GameRoot::instance().windowSizeF.x - 22.f, 25.f });
    highScoreHeaderText.setStyle(sf::Text::Bold);
    highScoreText.setStyle(sf::Text::Bold);
}


std::string UserInterface::formattedTime()
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


void UserInterface::draw()
{
    // // Draw the score text todo: uncomment
    // scoreFrame.setColor({255, 255, 255, static_cast<std::uint8_t>(255 * GameRoot::instance().frameUIOpacity)});
    // GameRoot::instance().renderWindow.draw(scoreFrame);
    // scoreText.setString(Extensions::formatNumberWithCommas(PlayerStatus::instance().score));
    // scoreText.setOrigin({
    //     scoreText.getLocalBounds().size.x / 2.f,
    //     scoreText.getLocalBounds().size.y / 2.f,
    // });
    // GameRoot::instance().renderWindow.draw(scoreText);
    //
    // // Draw the multiplier header and text
    // GameRoot::instance().renderWindow.draw(multiplierHeaderText);
    // multiplierText.setString(std::to_string(PlayerStatus::instance().multiplier));
    // multiplierText.setOrigin({
    //     multiplierText.getLocalBounds().size.x / 2.f,
    //     multiplierText.getLocalBounds().size.y / 2.f
    // });
    // GameRoot::instance().renderWindow.draw(multiplierText);
    //
    // // Draw the timer text
    // timerText.setString(formattedTime());
    // const sf::FloatRect timerTextRect = timerText.getLocalBounds();
    // float timerOriginTextX = timerTextRect.size.x / 2.f;
    // float timerOriginTextY = timerTextRect.size.y / 2.f;
    // timerText.setOrigin({timerOriginTextX, timerOriginTextY});
    // GameRoot::instance().renderWindow.draw(timerText);

    // Draw paused screen
    if (GameRoot::instance().isPaused)
    {
        GameRoot::instance().renderWindow.draw(pausedText);

        // Draw the high score header and text, right align the highscore number
        GameRoot::instance().renderWindow.draw(highScoreHeaderText);
        highScoreText.setString(Extensions::formatNumberWithCommas(PlayerStatus::instance().highScore));
        const sf::FloatRect highscoreTextRect = highScoreText.getLocalBounds();
        multiplierText.setOrigin({highscoreTextRect.size.x, highscoreTextRect.size.y});
        highScoreText.setPosition({ GameRoot::instance().windowSizeF.x - highscoreTextRect.size.x - 22.f, 30.f });
        GameRoot::instance().renderWindow.draw(highScoreText);
    }

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
