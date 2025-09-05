#ifndef BLOOM_H
#define BLOOM_H
#include "../../GameRoot.h"
#include "SFML/Graphics/RenderTexture.hpp"
#include "SFML/Graphics/Shader.hpp"
#include "SFML/Graphics/Sprite.hpp"


class Bloom {
private:
    const float bloomThreshold = 0.4f; // Smaller sigma leads to a faster decay
    const float blurAmount = 5.0f;
    const float bloomIntensity = 1.0f;
    const float baseIntensity = 1.1f;
    const float bloomSaturation = 1.0f;
    const float baseSaturation = 1.2f;

    float computeGaussian(float) const;
    void setGaussianBlurParameters(float, float);

public:
    Bloom();
    ~Bloom() = default;

    static Bloom &instance() {
        static auto *instance = new Bloom;
        return *instance;
    }

    bool bloomEnabled = true;
    sf::Shader bloomExtract;
    sf::Shader gaussianBlur;
    sf::Shader bloomCombine;
    sf::RenderTexture baseTexture {{GameRoot::instance().renderWindow.getSize().x, GameRoot::instance().renderWindow.getSize().y}};
    sf::RenderTexture bloomExtractTexture {{GameRoot::instance().renderWindow.getSize().x, GameRoot::instance().renderWindow.getSize().y}};
    sf::RenderTexture gaussianBlurTexture {{GameRoot::instance().renderWindow.getSize().x, GameRoot::instance().renderWindow.getSize().y}};

    void clearTextures();
    void drawToBaseBloomTexture(const sf::Sprite&);
    void drawBloomToScreen();
};



#endif //BLOOM_H
