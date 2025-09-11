#ifndef GAUSSIANBLUR_H
#define GAUSSIANBLUR_H
#include "../../GameRoot.h"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderTexture.hpp"
#include "SFML/Graphics/Shader.hpp"
#include "SFML/Graphics/Sprite.hpp"


class GaussianBlur {
private:
    const float saturationSigma = 0.5f;
    const float blurAmount = 4.f; // Smaller sigma leads to a faster decay
    bool gaussianBlurEnabled = true;

    float computeGaussian(float) const;
    void setGaussianBlurParameters(float, float);

public:
    GaussianBlur();
    ~GaussianBlur() = default;

    static GaussianBlur &instance() {
        static auto *instance = new GaussianBlur;
        return *instance;
    }

    sf::Shader saturate;
    sf::Shader gaussianBlur;
    sf::Shader blurSaturateCombine;
    sf::RenderStates gaussianRenderState;
    sf::RenderTexture baseTexture {{GameRoot::instance().renderWindow.getSize().x, GameRoot::instance().renderWindow.getSize().y}};
    sf::RenderTexture saturationTexture {{GameRoot::instance().renderWindow.getSize().x, GameRoot::instance().renderWindow.getSize().y}};
    sf::RenderTexture gaussianBlurTexture {{GameRoot::instance().renderWindow.getSize().x, GameRoot::instance().renderWindow.getSize().y}};

    void toggleGaussianBlur();
    void clearTextures();
    void drawToBase(const sf::Sprite&);
    void drawToBase(const std::vector<sf::Vertex>&);
    void drawToScreen();
};



#endif //GAUSSIANBLUR_H
