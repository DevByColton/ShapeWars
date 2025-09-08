#include <cmath>
#include <numbers>
#include "../Include/GaussianBlur.h"
#include "../../GameRoot.h"
#include "../../Logger/Logger.h"


GaussianBlur::GaussianBlur()
{
    // Load saturate shader
    if (!saturate.loadFromFile("Content\\Shaders\\Saturate.frag", sf::Shader::Type::Fragment))
    {
        gaussianBlurEnabled = false;
        Logger::printError("Unable to load Saturate.frag");
    }

    // Load gaussian blur shader
    if (!gaussianBlur.loadFromFile("Content\\Shaders\\GaussianBlur.frag", sf::Shader::Type::Fragment))
    {
        gaussianBlurEnabled = false;
        Logger::printError("Unable to load GaussianBlur.frag");
    }

    // Load blur saturate shader
    if (!blurSaturateCombine.loadFromFile("Content\\Shaders\\BlurSaturateCombine.frag", sf::Shader::Type::Fragment))
    {
        gaussianBlurEnabled = false;
        Logger::printError("Unable to load BlurSaturateCombine.frag");
    }

    // Make sure the textures are set to smooth for smooth blending
    baseTexture.setSmooth(true);
    saturationTexture.setSmooth(true);
    gaussianBlurTexture.setSmooth(true);

    // Set the gaussian render state to additive to preserve the saturation when blurring
    gaussianRenderState = {&gaussianBlur};
    gaussianRenderState.blendMode = sf::BlendAdd;
}


void GaussianBlur::clearTextures()
{
    baseTexture.clear();
    saturationTexture.clear();
    gaussianBlurTexture.clear();
}


void GaussianBlur::drawToBase(const sf::Sprite &sprite)
{
    baseTexture.draw(sprite);
    baseTexture.display();
}


void GaussianBlur::drawToScreen()
{
    if (gaussianBlurEnabled)
    {
        sf::Sprite baseTextureSprite {baseTexture.getTexture()};

        // Pass 1: Saturate
        saturate.setUniform("saturationSigma", saturationSigma);
        saturationTexture.draw(baseTextureSprite, &saturate);
        saturationTexture.display();
        const sf::Sprite saturationSprite{saturationTexture.getTexture()};

        // Pass 2: Horizontal gaussian blur
        const float dx = 1.0f / saturationTexture.getSize().x;
        setGaussianBlurParameters(dx, 0.0);
        gaussianBlurTexture.draw(saturationSprite, gaussianRenderState);
        gaussianBlurTexture.display();
        const sf::Sprite gaussianBlurSprite {gaussianBlurTexture.getTexture()};

        // Pass 3: Vertical gaussian blur, draw back into the saturation sprite
        const float dy = 1.0f / saturationTexture.getSize().y;
        setGaussianBlurParameters(0.0, dy);
        saturationTexture.draw(gaussianBlurSprite, gaussianRenderState);
        saturationTexture.display();

        // Final pass: Combine the saturation and blur textures with the base texture and draw to screen
        blurSaturateCombine.setUniform("baseTexture", baseTexture.getTexture());
        blurSaturateCombine.setUniform("saturationTexture", saturationTexture.getTexture());
        GameRoot::instance().renderWindow.draw(saturationSprite, &blurSaturateCombine);
    }
    else
    {
        const sf::Sprite baseTextureSprite {baseTexture.getTexture()};
        GameRoot::instance().renderWindow.draw(baseTextureSprite);
    }
}


void GaussianBlur::setGaussianBlurParameters(const float dx, const float dy)
{
    // Effect parameters
    constexpr int sampleCount = 15;
    float sampleWeights [sampleCount] {};
    sf::Glsl::Vec2 sampleOffsets [sampleCount] {};

    // The first sample always has a zero offset
    sampleWeights[0] = computeGaussian(0);
    sampleOffsets[0] = {0.0, 0.0};

    // Sum of all weighting values
    float totalWeight = sampleWeights[0];

    // Add pairs of additional sample taps, positioned
    // along a line in both directions from the center
    for (std::size_t i = 0; i < sampleCount / 2; i++)
    {
        // Store weights for the positive and negative taps
        const float weight = computeGaussian(i + 1);

        sampleWeights[i * 2 + 1] = weight;
        sampleWeights[i * 2 + 2] = weight;

        totalWeight += weight * 2;

        // To get the maximum amount of blurring from a limited number of
        // pixel shader samples, we take advantage of the bilinear filtering
        // hardware inside the texture fetch unit. If we position our texture
        // coordinates exactly halfway between two texels, the filtering unit
        // will average them for us, giving two samples for the price of one.
        // This allows us to step in units of two texels per sample, rather
        // than just one at a time. The 1.5 offset kicks things off by
        // positioning us nicely in between two texels
        const float sampleOffset = i * 2 + 1.5f;

        const sf::Glsl::Vec2 delta = {dx * sampleOffset, dy * sampleOffset};

        // Store texture coordinate offsets for the positive and negative taps
        sampleOffsets[i * 2 + 1] = delta;
        sampleOffsets[i * 2 + 2] = -delta;
    }

    // Normalize the list of sample weightings
    for (std::size_t i = 0; i < sampleCount; i++)
        sampleWeights[i] /= totalWeight;

    // Set the shader parameters
    gaussianBlur.setUniform("sampleCount", sampleCount);
    gaussianBlur.setUniformArray("sampleWeights", sampleWeights, sampleCount);
    gaussianBlur.setUniformArray("sampleOffsets", sampleOffsets, sampleCount);
}


float GaussianBlur::computeGaussian(const float n) const
{
    // Gaussian function
    const float x = 1.0f / std::sqrt(2 * std::numbers::pi * blurAmount);
    const float y = std::exp(-(n * n) / (2 * (blurAmount * blurAmount)));
    return x * y;
}

