#ifndef GAMEROOT_H
#define GAMEROOT_H
#include "SFML/Graphics/RenderWindow.hpp"


class GameRoot {
private:
    GameRoot();
    ~GameRoot() = default;

    sf::Clock deltaTimeClock {};
    sf::Clock totalGameTimeClock {};

public:
    static GameRoot &instance() {
        static auto *instance = new GameRoot;
        return *instance;
    }

    sf::RenderWindow renderWindow {};
    sf::Vector2f windowSizeF {0.0, 0.0};
    sf::Rect<float> windowRectangle {};
    bool isPaused = false;
    float deltaTime = 0.0f;

    sf::Vector2<int> size() const;
    float halfWidth() const;
    float halfHeight() const;
    float totalGameTime() const;
    void togglePause();
    void stopTotalGameClock();
    void restartTotalGameClock();
    void update();
};



#endif //GAMEROOT_H
