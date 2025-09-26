#ifndef GAMEROOT_H
#define GAMEROOT_H
#include <chrono>
#include "SFML/Graphics/RenderWindow.hpp"


class GameRoot {
private:
    GameRoot();
    ~GameRoot() = default;

    bool vsyncEnabled = true;
    const std::chrono::milliseconds MS_PER_FRAME {16};
    sf::Clock gameClock {};
    sf::Clock deltaClock {};

    std::chrono::milliseconds getCurrentTime();
    void toggleVsync();
    void togglePause();
    void processInput();
    void update();
    void render();

public:
    static GameRoot &instance() {
        static auto *instance = new GameRoot;
        return *instance;
    }

    sf::RenderWindow renderWindow {};
    sf::Vector2f windowSizeF {0.0, 0.0};
    sf::Rect<float> windowRectangle {};
    bool isPaused = false;

    float elapsedGameTime = 0.f;
    float deltaTime = 0.f;

    void run();
};



#endif //GAMEROOT_H
