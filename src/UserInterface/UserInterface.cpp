#include <numbers>
#include "UserInterface.h"
#include "../GameRoot.h"
#include "../PlayerStatus/PlayerStatus.h"
#include "../System/Include/Extensions.h"


UserInterface::UserInterface() {

    // Set the player ship sprite properties for lives counter
    livesText.setPosition({ 25.0f, 10.0f });
    float originX = playerShipSprite.getTexture().getSize().x / 2;
    float originY = playerShipSprite.getTexture().getSize().y / 2;
    playerShipSprite.setOrigin({originX, originY});
    playerShipSprite.setPosition({40.0f, 62.0f});
    playerShipSprite.setScale({0.75f, 0.75f});
    playerShipSprite.setRotation(sf::radians(-std::numbers::pi / 2));

    // Set the score to be in the top middle of the screen
    float scoreTextX = GameRoot::instance().windowSizeF.x / 2;
    scoreText.setPosition({scoreTextX, 28.0f});

    // Set the multiplier text to be in the right top of the window
    const sf::FloatRect multiplierHeaderTextRect = multiplierHeaderText.getLocalBounds();
    float multiplierOriginHeaderTextX = multiplierHeaderTextRect.size.x / 2.0f;
    float multiplierOriginHeaderTextY = multiplierHeaderTextRect.size.y / 2.0f;
    multiplierHeaderText.setOrigin({multiplierOriginHeaderTextX, multiplierOriginHeaderTextY});
    multiplierHeaderText.setPosition({ scoreText.getPosition().x, scoreText.getPosition().y + 60.0f });
    multiplierText.setPosition({ multiplierHeaderText.getPosition().x + 40.0f, multiplierHeaderText.getPosition().y });

    // Justify the pause text slightly under the multiplier text in the middle of the screen
    pausedText.setPosition({GameRoot::instance().windowSizeF.x / 2.0f, 250.0f});
    pausedText.setOrigin({ pausedText.getLocalBounds().size.x / 2.0f, pausedText.getLocalBounds().size.y / 2.0f });

    // Set the game over text in the middle of the screen
    gameOverText.setPosition({GameRoot::instance().windowSizeF.x / 2.0f, GameRoot::instance().windowSizeF.y / 2.0f});

    // Set the highscore text to be in the top right of the window, right aligned
    const sf::FloatRect highscoreHeaderTextRect = highScoreHeaderText.getLocalBounds();
    highScoreHeaderText.setOrigin({ highscoreHeaderTextRect.size.x, highscoreHeaderTextRect.size.y});
    highScoreHeaderText.setPosition({ GameRoot::instance().windowSizeF.x - 22.0f, 25.0f });

}


float UserInterface::playerShipWidthScaled() const {
    const float widthWithPadding = playerShipSprite.getTexture().getSize().x + 16;
    return widthWithPadding * playerShipSprite.getScale().x;
}


void UserInterface::draw() {

    // Draw the number of lives in the top left of the screen
    GameRoot::instance().renderWindow.draw(livesText);
    for (std::size_t i = 0; i < PlayerStatus::instance().lives; i++) {

        sf::Sprite nextSprite = playerShipSprite;
        sf::Vector2f basePositon = playerShipSprite.getPosition();
        nextSprite.setPosition({ basePositon.x + i * playerShipWidthScaled(), basePositon.y });
        GameRoot::instance().renderWindow.draw(nextSprite);

    }

    // Draw the score text
    scoreText.setString(formatNumberWithCommas(PlayerStatus::instance().score));
    const sf::FloatRect scoreTextRect = scoreText.getLocalBounds();
    float scoreTextOriginX = scoreTextRect.size.x / 2.0f;
    float scoreTextOriginY = scoreTextRect.size.y / 2.0f;
    scoreText.setOrigin({scoreTextOriginX, scoreTextOriginY});
    GameRoot::instance().renderWindow.draw(scoreText);

    // Draw the multiplier header and text
    GameRoot::instance().renderWindow.draw(multiplierHeaderText);
    multiplierText.setString(std::to_string(PlayerStatus::instance().multiplier));
    const sf::FloatRect multiplierTextRect = multiplierText.getLocalBounds();
    float multiplierOriginTextX = multiplierTextRect.size.x / 2.0f;
    float multiplierOriginTextY = multiplierTextRect.size.y / 2.0f;
    multiplierText.setOrigin({multiplierOriginTextX, multiplierOriginTextY});
    GameRoot::instance().renderWindow.draw(multiplierText);

    // Draw the high score header and text, right align the highscore number
    GameRoot::instance().renderWindow.draw(highScoreHeaderText);
    highScoreText.setString(formatNumberWithCommas(PlayerStatus::instance().highScore));
    const sf::FloatRect highscoreTextRect = highScoreText.getLocalBounds();
    multiplierText.setOrigin({highscoreTextRect.size.x, highscoreTextRect.size.y});
    highScoreText.setPosition({ GameRoot::instance().windowSizeF.x - highscoreTextRect.size.x - 22.0f, 30.0f });
    GameRoot::instance().renderWindow.draw(highScoreText);

    // Draw paused text
    if (GameRoot::instance().isPaused)
        GameRoot::instance().renderWindow.draw(pausedText);

    // Draw game over text
    if (PlayerStatus::instance().isGameOver()) {
        std::string text {"== [ game over ]== \n"};
        text.append("your score: ");
        text.append(formatNumberWithCommas(PlayerStatus::instance().score) + "\n");
        text.append("high score: ");
        text.append(formatNumberWithCommas(PlayerStatus::instance().highScore));
        gameOverText.setString(text);
        gameOverText.setOrigin({gameOverText.getLocalBounds().size.x / 2.0f, gameOverText.getLocalBounds().size.y / 2.0f});
        GameRoot::instance().renderWindow.draw(gameOverText);
    }

}
