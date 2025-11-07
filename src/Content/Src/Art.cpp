#include "../Include/Art.h"
#include "../../Core/Include/Logger.h"
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

        laser = sf::Texture("Content\\Images\\Laser.png");
        laser.setSmooth(true);

        shapeKeeperCoreShield = sf::Texture("Content\\Images\\ShapeKeeperCoreShielded.png");
        shapeKeeperCoreShield.setSmooth(true);

        shapeKeeperCore = sf::Texture("Content\\Images\\ShapeKeeperCore.png");
        shapeKeeperCore.setSmooth(true);

        shapeKeeperCoreTrail = sf::Texture("Content\\Images\\ShapeKeeperCoreTrail.png");
        shapeKeeperCoreTrail.setSmooth(true);

        shapeKeeperTop = sf::Texture("Content\\Images\\ShapeKeeperTop.png");
        shapeKeeperTop.setSmooth(true);

        shapeKeeperTopTrail = sf::Texture("Content\\Images\\ShapeKeeperTopTrail.png");
        shapeKeeperTopTrail.setSmooth(true);

        shapeKeeperMiddleLeft = sf::Texture("Content\\Images\\ShapeKeeperMiddleLeft.png");
        shapeKeeperMiddleLeft.setSmooth(true);

        shapeKeeperMiddleLeftTrail = sf::Texture("Content\\Images\\ShapeKeeperMiddleLeftTrail.png");
        shapeKeeperMiddleLeftTrail.setSmooth(true);

        shapeKeeperMiddleRight = sf::Texture("Content\\Images\\ShapeKeeperMiddleRight.png");
        shapeKeeperMiddleRight.setSmooth(true);

        shapeKeeperMiddleRightTrail = sf::Texture("Content\\Images\\ShapeKeeperMiddleRightTrail.png");
        shapeKeeperMiddleRightTrail.setSmooth(true);

        shapeKeeperBottomLeft = sf::Texture("Content\\Images\\ShapeKeeperBottomLeft.png");
        shapeKeeperBottomLeft.setSmooth(true);

        shapeKeeperBottomLeftTrail = sf::Texture("Content\\Images\\ShapeKeeperBottomLeftTrail.png");
        shapeKeeperBottomLeftTrail.setSmooth(true);

        shapeKeeperBottomRight = sf::Texture("Content\\Images\\ShapeKeeperBottomRight.png");
        shapeKeeperBottomRight.setSmooth(true);

        shapeKeeperBottomRightTrail = sf::Texture("Content\\Images\\ShapeKeeperBottomRightTrail.png");
        shapeKeeperBottomRightTrail.setSmooth(true);

        shapeKeeperHealthBarCore = sf::Texture("Content\\Images\\ShapeKeeperHealthBarCore.png");
        shapeKeeperHealthBarCore.setSmooth(true);

        shapeKeeperHealthBarTop = sf::Texture("Content\\Images\\ShapeKeeperHealthBarTop.png");
        shapeKeeperHealthBarTop.setSmooth(true);

        shapeKeeperHealthBarMiddleLeft = sf::Texture("Content\\Images\\ShapeKeeperHealthBarMiddleLeft.png");
        shapeKeeperHealthBarMiddleLeft.setSmooth(true);

        shapeKeeperHealthBarMiddleRight = sf::Texture("Content\\Images\\ShapeKeeperHealthBarMiddleRight.png");
        shapeKeeperHealthBarMiddleRight.setSmooth(true);

        shapeKeeperHealthBarBottomLeft = sf::Texture("Content\\Images\\ShapeKeeperHealthBarBottomLeft.png");
        shapeKeeperHealthBarBottomLeft.setSmooth(true);

        shapeKeeperHealthBarBottomRight = sf::Texture("Content\\Images\\ShapeKeeperHealthBarBottomRight.png");
        shapeKeeperHealthBarBottomRight.setSmooth(true);

        shapeKeeperHealthFrame = sf::Texture("Content\\Images\\ShapeKeeperHealthFrame.png");
        shapeKeeperHealthFrame.setSmooth(true);

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

        escape = sf::Texture("Content\\Images\\Escape.png");
        escape.setSmooth(true);

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

        switchChecked = sf::Texture("Content\\Images\\SwitchChecked.png");
        switchChecked.setSmooth(true);

        switchUnchecked = sf::Texture("Content\\Images\\SwitchUnchecked.png");
        switchUnchecked.setSmooth(true);

        sliderOptionKnob = sf::Texture("Content\\Images\\SliderOptionKnob.png");
        sliderOptionKnob.setSmooth(true);

        sliderOptionBarFill = sf::Texture("Content\\Images\\SliderOptionBarFill.png");
        sliderOptionBarFill.setSmooth(true);

        sliderOptionBarOutline = sf::Texture("Content\\Images\\SliderOptionBarOutline.png");
        sliderOptionBarOutline.setSmooth(true);

        turretRoadFont = sf::Font("Content\\Font\\TurretRoad-Regular.ttf");
        turretRoadFont.setSmooth(true);

        majorMonoFont = sf::Font("Content\\Font\\MajorMonoDisplay-Regular.ttf");
        majorMonoFont.setSmooth(true);

    }
    catch (sf::Exception &ex)
    {
        Logger::printError(ex.what());
    }
}
