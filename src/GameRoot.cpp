#include <string>
#include "GameRoot.h"
#include "Logger/Logger.h"

GameRoot::GameRoot() {

    // Set fullscreen mode and set the window maximum size to it
    const auto& fullscreenModes = sf::VideoMode::getFullscreenModes();
    const unsigned int width = fullscreenModes[0].size.x;
    const unsigned int height = fullscreenModes[0].size.y;
    sf::Vector2 maxWindowSize {width, height};
    const unsigned int bitsPerPixel = fullscreenModes[0].bitsPerPixel;

    // Create the render window with fullscreen, 60 fps, and properties
    renderWindow = sf::RenderWindow(
        sf::VideoMode(maxWindowSize, bitsPerPixel),
        "Shape Wars",
        sf::Style::Default,
        sf::State::Fullscreen
    );
    renderWindow.setMaximumSize(maxWindowSize);
    renderWindow.setFramerateLimit(60);
    renderWindow.setMouseCursorVisible(false);

    // Set the screen size in float for easy maths
    windowSizeF = {static_cast<float>(width), static_cast<float>(height)};
    windowRectangle = {{0.0, 0.0}, {windowSizeF.x, windowSizeF.y}};

    Logger::printOut(
        "Created fullscreen window with size " + std::to_string(width) +
        " x " + std::to_string(height) + " and " +
        std::to_string(bitsPerPixel) + " bits per pixel"
    );
}

sf::Vector2<int> GameRoot::size() const {
    return sf::Vector2<int>(renderWindow.getSize());
}

float GameRoot::halfWidth() const {
    return static_cast<float>(renderWindow.getSize().x / 2.0);
}

float GameRoot::halfHeight() const {
    return static_cast<float>(renderWindow.getSize().y / 2.0);
}



