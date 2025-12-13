#ifndef GAMEROOT_H
#define GAMEROOT_H
#include <chrono>
#include "GameState/Include/IGameState.h"
#include "SFML/Graphics/RenderWindow.hpp"


class GameRoot {
private:
    GameRoot();

    static constexpr std::chrono::milliseconds MS_PER_FRAME {16};
    sf::Clock gameClock {};
    sf::Clock deltaClock {};

    static constexpr int MAX_GAME_STATE_COUNT = 3;
    IGameState* activeInputState = {nullptr};
    std::array<IGameState*, MAX_GAME_STATE_COUNT> updatableGameStates {nullptr, nullptr, nullptr};
    std::array<IGameState*, MAX_GAME_STATE_COUNT> drawableGameStates {nullptr, nullptr, nullptr};

    void setupDebug();
    void setupRelease();
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

    void setActiveInputState(IGameState* gameState);
    void addUpdatableState(IGameState* gameState);
    void removeUpdatableState(const IGameState* gameState);
    void addDrawableState(IGameState* gameState);
    void removeDrawableState(const IGameState* gameState);
    void load();
    void run();
};



#endif //GAMEROOT_H
