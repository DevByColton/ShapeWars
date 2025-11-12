#include "../Include/Background.h"
#include "../../GameRoot.h"

Background::Background()
{
    topLeftNotch[0].position = sf::Vector2f(20.f, 20.f);
    topLeftNotch[1].position = sf::Vector2f(20.f, 75.f);
    topLeftNotch[2].position = sf::Vector2f(75.f, 20.f);
    topLeftNotch[0].color = NOTCH_COLOR;
    topLeftNotch[1].color = NOTCH_COLOR;
    topLeftNotch[2].color = NOTCH_COLOR;

    topRightNotch[0].position = sf::Vector2f(GameRoot::instance().windowSizeF.x - 20.f, 20.f);
    topRightNotch[1].position = sf::Vector2f(GameRoot::instance().windowSizeF.x - 20.f, 75.f);
    topRightNotch[2].position = sf::Vector2f(GameRoot::instance().windowSizeF.x - 75.f, 20.f);
    topRightNotch[0].color = NOTCH_COLOR;
    topRightNotch[1].color = NOTCH_COLOR;
    topRightNotch[2].color = NOTCH_COLOR;

    bottomRightNotch[0].position = sf::Vector2f(GameRoot::instance().windowSizeF.x - 20.f, GameRoot::instance().windowSizeF.y - 20.f);
    bottomRightNotch[1].position = sf::Vector2f(GameRoot::instance().windowSizeF.x - 20.f, GameRoot::instance().windowSizeF.y - 75.f);
    bottomRightNotch[2].position = sf::Vector2f(GameRoot::instance().windowSizeF.x - 75.f, GameRoot::instance().windowSizeF.y - 20.f);
    bottomRightNotch[0].color = NOTCH_COLOR;
    bottomRightNotch[1].color = NOTCH_COLOR;
    bottomRightNotch[2].color = NOTCH_COLOR;

    bottomLeftNotch[0].position = sf::Vector2f(20.f, GameRoot::instance().windowSizeF.y - 20.f);
    bottomLeftNotch[1].position = sf::Vector2f(20.f, GameRoot::instance().windowSizeF.y - 75.f);
    bottomLeftNotch[2].position = sf::Vector2f(75.f, GameRoot::instance().windowSizeF.y - 20.f);
    bottomLeftNotch[0].color = NOTCH_COLOR;
    bottomLeftNotch[1].color = NOTCH_COLOR;
    bottomLeftNotch[2].color = NOTCH_COLOR;
}


void Background::setVertexData(const int index, const sf::Vector2f& position, const sf::Color& color)
{
    backgroundVertices[index].position = position;
    backgroundVertices[index].color = color;
}


void Background::draw(sf::RenderTexture& renderTexture) const
{
    renderTexture.draw(backgroundVertices);
    renderTexture.draw(topLeftNotch);
    renderTexture.draw(topRightNotch);
    renderTexture.draw(bottomRightNotch);
    renderTexture.draw(bottomLeftNotch);
}
