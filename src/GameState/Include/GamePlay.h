#ifndef GAMEPLAY_H
#define GAMEPLAY_H
#include "IGameState.h"
#include "../UI/Include/GamePlayHUD.h"

enum GamePlayState
{
    PreBoss,
    BossFight,
    Endless
};


class GamePlay final : IGameState
{
private:
    GamePlayState currentGamePlayState = PreBoss;

public:
    GamePlay();

    static GamePlay &instance()
    {
        static auto *instance = new GamePlay;
        return *instance;
    }

    bool isShapeKeeperEncounterStarting = false;

    bool markRoundStart = false;
    bool markShapeKeeperStart = false;
    bool markShapeKeeperEnd = false;
    bool markRoundEnd = false;

    void doBaseReset();
    void doTotalReset();
    void startRound();
    void endRound();
    void processMouseReleased(const sf::Event::MouseButtonReleased* mouseReleased) override;
    void processKeyReleased(const sf::Event::KeyReleased* keyReleased) override;
    void processJoystickButtonReleased(const sf::Event::JoystickButtonReleased* joystickButtonReleased) override;
    void processJoystickAxisMoved(const sf::Event::JoystickMoved* joystickMoved) override;
    void update() override;
    void renderGaussianBlur() override;
    void renderToScreen() override;
};



#endif //GAMEPLAY_H
