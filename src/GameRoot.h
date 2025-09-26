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

    void toggleVsync();
    void togglePause();
    void processInput();
    void update();
    void render();
    std::chrono::milliseconds getCurrentTime();

public:
    static GameRoot &instance() {
        static auto *instance = new GameRoot;
        return *instance;
    }

    float elapsedGameTime = 0.f;
    float deltaTime = 0.f;
    bool isPaused = false;
    sf::RenderWindow renderWindow {};
    sf::Vector2f windowSizeF {0.0, 0.0};
    sf::Rect<float> windowRectangle {};

    void run();
};



#endif //GAMEROOT_H
