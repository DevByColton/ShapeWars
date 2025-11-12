#include "../Include/MenuOption.h"


MenuOption::MenuOption(const sf::Font& font, const sf::String& string, const unsigned int characterSize): sf::Text(font, string, characterSize)
{
    // Mute the menu option by default
    setFillColor(MUTED_TEXT_COLOR);
    setStyle(Bold);
}
