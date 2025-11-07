#ifndef GAMEPLAYHUD_H
#define GAMEPLAYHUD_H
#include "GamePlayControlArea.h"
#include "../../../GameRoot.h"
#include "../../../Content/Include/Art.h"
#include "../../../Entities/Include/ShapeKeeper/ShapeKeeperHealthContainer.h"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Graphics/RenderTexture.hpp"


struct GamePlayHUD {
    GamePlayHUD();

    static GamePlayHUD &instance()
    {
        static auto *instance = new GamePlayHUD;
        return *instance;
    }

    static constexpr float TRANSITION_DURATION = 0.5f;
    const std::string SCORE_OBJECTIVE_TEXT {"==[ Achieve 250,000 points... ]=="};
    const std::string ENDLESS_OBJECTIVE_TEXT {"==[ War never ends! ]=="};
    const sf::Vector2f scoreAreaOffScreenPosition {GameRoot::instance().windowSizeF.x / 2.f, -75.f};
    const sf::Vector2f scoreAreaOnScreenPosition {GameRoot::instance().windowSizeF.x / 2.f, 55.f};
    bool isTransitioningScoreAreaIn = false;
    bool isTransitioningScoreAreaOut = false;
    bool shouldStartHealthArea = false;
    bool shouldStartScoreArea = false;
    float transitionTime = 0.f;
    sf::Sprite scoreFrame {Art::instance().scoreFrame};
    sf::RenderTexture scoreAreaTexture {{scoreFrame.getTexture().getSize().x, scoreFrame.getTexture().getSize().y}};
    sf::Sprite scoreArea {scoreAreaTexture.getTexture()};
    sf::Text scoreText {Art::instance().turretRoadFont, {}, 60};
    sf::Text multiplierHeaderText {Art::instance().turretRoadFont, {"mult x "}, 20};
    sf::Text multiplierText {Art::instance().turretRoadFont, {}, 20};
    sf::Text timerText {Art::instance().turretRoadFont, {}, 20};
    sf::Text objectiveText {Art::instance().turretRoadFont, SCORE_OBJECTIVE_TEXT, 17};
    sf::Text gameOverText {Art::instance().turretRoadFont, {}, 60};
    ShapeKeeperHealthContainer healthContainer {};
    GamePlayControlArea gamePlayControlArea {};

    void resetObjective();
    void setObjectiveEndless();
    void markHealthAreaTransitionOut(bool shouldStartScoreArea);
    void markScoreAreaTransitionOut(bool shouldStartHealthArea);
    bool transitionInScoreArea();
    bool transitionOutScoreArea();
    static std::string formattedTime();
    void update();
    void drawToBlur();
    void drawToScreen();
};



#endif //GAMEPLAYHUD_H
