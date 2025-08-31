#ifndef GAMEROOT_H
#define GAMEROOT_H
#include "SFML/Graphics/RenderWindow.hpp"


class GameRoot {
private:
    GameRoot();
    ~GameRoot() = default;

public:
    static GameRoot &instance() {
        static auto *instance = new GameRoot;
        return *instance;
    }

    sf::Clock gameClock {};
    sf::RenderWindow renderWindow {};
    sf::Vector2f windowSizeF {0.0, 0.0};
    sf::Rect<float> windowRectangle {};
    bool isPaused = false;
    float deltaTime = 0.0f;

    sf::Vector2<int> size() const;
    float halfWidth() const;
    float halfHeight() const;
    void togglePause();
    void update();
};



#endif //GAMEROOT_H
