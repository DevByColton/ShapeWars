#ifndef BACKGROUND_H
#define BACKGROUND_H
#include "SFML/Graphics/PrimitiveType.hpp"
#include "SFML/Graphics/RenderTexture.hpp"
#include "SFML/Graphics/VertexArray.hpp"


struct Background {
    Background();

    const sf::Color NOTCH_COLOR = {95, 92, 100, 50};
    sf::VertexArray backgroundVertices {sf::PrimitiveType::TriangleStrip, 4};
    sf::VertexArray topLeftNotch {sf::PrimitiveType::Triangles, 3};
    sf::VertexArray topRightNotch {sf::PrimitiveType::Triangles, 3};
    sf::VertexArray bottomRightNotch {sf::PrimitiveType::Triangles, 3};
    sf::VertexArray bottomLeftNotch {sf::PrimitiveType::Triangles, 3};

    void setVertexData(int index, const sf::Vector2f& position, const sf::Color& color);
    void draw(sf::RenderTexture& renderTexture) const;
};



#endif //BACKGROUND_H
