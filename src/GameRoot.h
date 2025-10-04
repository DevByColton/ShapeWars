#ifndef GAMEROOT_H
#define GAMEROOT_H
#include <chrono>
#include "SFML/Graphics/RenderWindow.hpp"


class GameRoot {
private:
    GameRoot();

    bool vsyncEnabled = true;
    const std::chrono::milliseconds MS_PER_FRAME {16};
    sf::Clock gameClock {};
    sf::Clock deltaClock {};

    void toggleVsync();
    void togglePause();
    void processKeyPressed(const sf::Event::KeyPressed* keyPressed);
    void processKeyReleased(const sf::Event::KeyReleased* keyReleased);
    void processJoystickButtonPressed(const sf::Event::JoystickButtonPressed* joystickButtonPressed);
    void processJoystickButtonReleased(const sf::Event::JoystickButtonReleased* joystickButtonReleased);
    void processJoystickAxisMoved(const sf::Event::JoystickMoved* joystickMoved);
    void processInput();
    void update() const;
    void render();
    std::chrono::milliseconds getCurrentTime();

public:
    static GameRoot &instance() {
        static auto *instance = new GameRoot;
        return *instance;
    }

    float frameUIOpacity = 0.35f;
    float elapsedGameTime = 0.f;
    float deltaTime = 0.f;
    bool isPaused = false;
    sf::RenderWindow renderWindow {};
    sf::Rect<float> windowRectangle {};
    sf::Vector2f windowSizeF {0.f, 0.f};
    sf::Vector2f topLeftCorner {0.f, 0.f};
    sf::Vector2f topRightCorner {0.f, 0.f};
    sf::Vector2f bottomRightCorner {0.f, 0.f};
    sf::Vector2f bottomLeftCorner {0.f, 0.f};

    void run();
};



#endif //GAMEROOT_H
