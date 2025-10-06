#include "../Include/Art.h"
#include "../../System/Include/Logger.h"
#include "SFML/System/Exception.hpp"

Art::Art()
{
    try
    {
        player = sf::Texture("Content\\Images\\Player.png");
        player.setSmooth(true);

        playerBoosters = sf::Texture("Content\\Images\\PlayerBoosters.png");
        playerBoosters.setSmooth(true);

        nuke = sf::Texture("Content\\Images\\Nuke.png");
        nuke.setSmooth(true);

        enemyPlaceholder = sf::Texture("Content\\Images\\EnemyPlaceholder.png");
        enemyPlaceholder.setSmooth(true);

        seeker = sf::Texture("Content\\Images\\Seeker.png");
        seeker.setSmooth(true);

        wanderer = sf::Texture("Content\\Images\\Wanderer.png");
        wanderer.setSmooth(true);

        dodger = sf::Texture("Content\\Images\\Dodger.png");
        dodger.setSmooth(true);

        snakeHead = sf::Texture("Content\\Images\\SnakeHead.png");
        snakeHead.setSmooth(true);

        snakeBody = sf::Texture("Content\\Images\\SnakeBody.png");
        snakeBody.setSmooth(true);

        snakeTail = sf::Texture("Content\\Images\\SnakeTail.png");
        snakeTail.setSmooth(true);

        bullet = sf::Texture("Content\\Images\\Bullet.png");
        bullet.setSmooth(true);

        pointer = sf::Texture("Content\\Images\\Cursor.png");
        pointer.setSmooth(true);

        blackHole = sf::Texture("Content\\Images\\BlackHole.png");
        blackHole.setSmooth(true);

        particle = sf::Texture("Content\\Images\\Particle.png");
        particle.setSmooth(true);

        particleGlow = sf::Texture("Content\\Images\\Glow.png");
        particleGlow.setSmooth(true);

        buffFrame = sf::Texture("Content\\Images\\BuffFrame.png");
        buffFrame.setSmooth(true);

        buffIconPlaceHolder = sf::Texture("Content\\Images\\BuffIconPlaceHolder.png");
        buffIconPlaceHolder.setSmooth(true);

        bulletsAllDirectionsBuff = sf::Texture("Content\\Images\\BulletsAllDirectionsBuff.png");
        bulletsAllDirectionsBuff.setSmooth(true);

        shotGunBuff = sf::Texture("Content\\Images\\ShotGunBuff.png");
        shotGunBuff.setSmooth(true);

        invincibleBuff = sf::Texture("Content\\Images\\InvincibleBuff.png");
        invincibleBuff.setSmooth(true);

        boostersBuff = sf::Texture("Content\\Images\\BoostersBuff.png");
        boostersBuff.setSmooth(true);

        livesAndNukesFrame = sf::Texture("Content\\Images\\LivesAndNukesFrame.png");
        livesAndNukesFrame.setSmooth(true);

        scoreFrame = sf::Texture("Content\\Images\\ScoreFrame.png");
        scoreFrame.setSmooth(true);

        keyboardFrame = sf::Texture("Content\\Images\\KeyboardFrame.png");
        keyboardFrame.setSmooth(true);

        num1 = sf::Texture("Content\\Images\\Num1.png");
        num1.setSmooth(true);

        num2 = sf::Texture("Content\\Images\\Num2.png");
        num2.setSmooth(true);

        num3 = sf::Texture("Content\\Images\\Num3.png");
        num3.setSmooth(true);

        num4 = sf::Texture("Content\\Images\\Num4.png");
        num4.setSmooth(true);

        spaceBar = sf::Texture("Content\\Images\\SpaceBar.png");
        spaceBar.setSmooth(true);

        dualsenseXButton = sf::Texture("Content\\Images\\DualSenseXButton.png");
        dualsenseXButton.setSmooth(true);

        dualsenseCircleButton = sf::Texture("Content\\Images\\DualSenseCircleButton.png");
        dualsenseCircleButton.setSmooth(true);

        dualsenseSquareButton = sf::Texture("Content\\Images\\DualSenseSquareButton.png");
        dualsenseSquareButton.setSmooth(true);

        dualsenseTriangleButton = sf::Texture("Content\\Images\\DualSenseTriangleButton.png");
        dualsenseTriangleButton.setSmooth(true);

        dualsenseRightTrigger = sf::Texture("Content\\Images\\DualSenseRightTrigger.png");
        dualsenseRightTrigger.setSmooth(true);

        xboxAButton = sf::Texture("Content\\Images\\XboxAButton.png");
        xboxAButton.setSmooth(true);

        xboxBButton = sf::Texture("Content\\Images\\XboxBButton.png");
        xboxBButton.setSmooth(true);

        xboxXButton = sf::Texture("Content\\Images\\XboxXButton.png");
        xboxXButton.setSmooth(true);

        xboxYButton = sf::Texture("Content\\Images\\XboxYButton.png");
        xboxYButton.setSmooth(true);

        xboxRightTrigger = sf::Texture("Content\\Images\\XboxRightTrigger.png");
        xboxRightTrigger.setSmooth(true);

        font = sf::Font("Content\\Font\\TurretRoad-Regular.ttf");
        font.setSmooth(true);

    }
    catch (sf::Exception &ex)
    {
        Logger::printError(ex.what());
    }
}
