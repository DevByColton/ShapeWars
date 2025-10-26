#ifndef GAMEROOT_H
#define GAMEROOT_H
#include <chrono>
#include "GameState/Include/GameState.h"
#include "SFML/Graphics/RenderWindow.hpp"


class GameRoot {
private:
    GameRoot();

    static constexpr std::chrono::milliseconds MS_PER_FRAME {16};
    bool vsyncEnabled = true;
    sf::Clock gameClock {};
    sf::Clock deltaClock {};
    GameState currentGameState = InStartMenu;

    void toggleVsync();
    void processInput();
    void update() const;
    void render();
    static std::chrono::milliseconds getCurrentTime();

public:
    static GameRoot &instance() {
        static auto *instance = new GameRoot;
        return *instance;
    }

    float frameUIOpacity = 0.7f;
    float elapsedGameTime = 0.f;
    float deltaTime = 0.f;
    sf::RenderWindow renderWindow {};
    sf::Rect<float> windowRectangle {};
    sf::Vector2f windowSizeF {0.f, 0.f};
    sf::Vector2f topLeftCorner {0.f, 0.f};
    sf::Vector2f topRightCorner {0.f, 0.f};
    sf::Vector2f bottomRightCorner {0.f, 0.f};
    sf::Vector2f bottomLeftCorner {0.f, 0.f};

    bool isCurrentGameState(GameState gameState) const;
    void setCurrentGameState(GameState nextGameState);
    void run();
};



#endif //GAMEROOT_H
