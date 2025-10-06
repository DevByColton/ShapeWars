#ifndef MOUSEANDKEYBOARD_H
#define MOUSEANDKEYBOARD_H
#include "../../Content/Include/Art.h"
#include "SFML/Graphics/Sprite.hpp"


class MouseAndKeyboard {
private:
    sf::Sprite mouseCursor{Art::instance().pointer};

public:
    static MouseAndKeyboard &instance()
    {
        static auto *instance = new MouseAndKeyboard;
        return *instance;
    }

    sf::Vector2f movementDirection();
    sf::Vector2f keysAimDirection();
    sf::Vector2f mouseAimDirection(const sf::Vector2f& fromPosition) const;
    void update();
    void draw() const;
};



#endif //MOUSEANDKEYBOARD_H
