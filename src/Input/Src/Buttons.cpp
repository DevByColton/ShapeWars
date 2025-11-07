#include "../Include/Buttons.h"
#include "../Include/Input.h"


Buttons::Button::Button(const sf::Texture& texture, const ButtonsOverride type) : Sprite(texture)
{
    setOrigin(getLocalBounds().getCenter());
    this->type = type;
}


void Buttons::drawXbox(sf::RenderTexture& renderTexture) const
{
    for (std::size_t bi = 0; bi < activeButtons.size(); ++bi)
        if (activeButtons.at(bi) != nullptr && activeButtons.at(bi)->type == ButtonsOverride::Xbox)
            renderTexture.draw(*activeButtons.at(bi));
}


void Buttons::drawDualsense(sf::RenderTexture& renderTexture) const
{
    for (std::size_t bi = 0; bi < activeButtons.size(); ++bi)
        if (activeButtons.at(bi) != nullptr && activeButtons.at(bi)->type == ButtonsOverride::Dualsense)
            renderTexture.draw(*activeButtons.at(bi));
}


void Buttons::drawKeyboard(sf::RenderTexture& renderTexture) const
{
    for (std::size_t bi = 0; bi < activeButtons.size(); ++bi)
        if (activeButtons.at(bi) != nullptr && activeButtons.at(bi)->type == ButtonsOverride::Keyboard)
            renderTexture.draw(*activeButtons.at(bi));
}


void Buttons::draw(sf::RenderTexture& renderTexture) const
{
    // If button override is not active, automatically handle button drawing
    if (!Input::instance().isButtonsOverrideActive)
    {
        // Favor xbox buttons if there is a valid xbox input
        if (Input::instance().hasValidXboxInput)
        {
            drawXbox(renderTexture);
            return;
        }

        // Dualsense
        if (Input::instance().hasValidDualsenseInput)
        {
            drawDualsense(renderTexture);
            return;
        }

        // Always draw keys as a fallthrough
        drawKeyboard(renderTexture);

        return;
    }

    // Button override is active
    if (Input::instance().buttonsOverride == ButtonsOverride::Xbox)
    {
        drawXbox(renderTexture);
        return;
    }

    if (Input::instance().buttonsOverride == ButtonsOverride::Dualsense)
    {
        drawDualsense(renderTexture);
        return;
    }

    if (Input::instance().buttonsOverride == ButtonsOverride::Keyboard)
        drawKeyboard(renderTexture);
}
