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

    sf::RenderWindow renderWindow {};
    sf::Vector2f windowSizeF {0.0, 0.0};
    sf::Rect<float> windowRectangle {};
    sf::Vector2<int> size() const;
    float halfWidth() const;
    float halfHeight() const;
};



#endif //GAMEROOT_H
