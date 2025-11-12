#ifndef MENUOPTION_H
#define MENUOPTION_H
#include <functional>
#include "SFML/Graphics/Text.hpp"


struct MenuOption final : sf::Text
{
    explicit MenuOption(const sf::Font& font, const sf::String& string, unsigned int characterSize);

    static constexpr sf::Color MUTED_TEXT_COLOR {185, 185, 185, 255};
    std::function<void()> onSelect {};
};



#endif //MENUOPTION_H
