#include <cmath>
#include <numbers>
#include "../Include/Bloom.h"
#include "../../GameRoot.h"
#include "../../Logger/Logger.h"


Bloom::Bloom()
{
    // Load bloom extract shader and set properties on success
    if (!bloomExtract.loadFromFile("Content\\Shaders\\BloomExtract.frag", sf::Shader::Type::Fragment))
    {
        bloomEnabled = false;
        Logger::printError("Unable to load BloomExtract.frag");
    }

    // Load gaussian blur shader
    if (!gaussianBlur.loadFromFile("Content\\Shaders\\GaussianBlur.frag", sf::Shader::Type::Fragment))
    {
        bloomEnabled = false;
        Logger::printError("Unable to load GaussianBlur.frag");
    }

    // Load bloom combine shader and set properties on success
    if (!bloomCombine.loadFromFile("Content\\Shaders\\BloomCombine.frag", sf::Shader::Type::Fragment))
    {
        bloomEnabled = false;
        Logger::printError("Unable to load BloomCombine.frag");
    }
}


void Bloom::clearTextures()
{
    baseTexture.clear();
    bloomExtractTexture.clear();
    gaussianBlurTexture.clear();
}


void Bloom::drawToBaseBloomTexture(const sf::Sprite &sprite)
{
    baseTexture.draw(sprite);
    baseTexture.display();
}


void Bloom::drawBloomToScreen()
{
    if (bloomEnabled)
    {
        sf::Sprite baseTextureSprite {baseTexture.getTexture()};

        // Pass 1: Extract bright parts (saturate)
        bloomExtract.setUniform("bloomThreshold", bloomThreshold);
        bloomExtractTexture.draw(baseTextureSprite, &bloomExtract);
        bloomExtractTexture.display();
        const sf::Sprite bloomExtractSprite{bloomExtractTexture.getTexture()};

        // Pass 2: Horizontal gaussian blur
        const float dx = 1.0f / bloomExtractTexture.getSize().x;
        setGaussianBlurParameters(dx, 0.0);
        gaussianBlurTexture.draw(bloomExtractSprite, &gaussianBlur);
        gaussianBlurTexture.display();
        const sf::Sprite gaussianSprite {gaussianBlurTexture.getTexture()};

        // Pass 3: Vertical gaussian blur
        const float dy = 1.0f / bloomExtractTexture.getSize().y;
        setGaussianBlurParameters(0.0, dy);
        bloomExtractTexture.draw(gaussianSprite, &gaussianBlur);
        bloomExtractTexture.display();

        // Final pass: Combine the extract and blur textures with the base texture
        bloomCombine.setUniform("bloomIntensity", bloomIntensity);
        bloomCombine.setUniform("baseIntensity", baseIntensity);
        bloomCombine.setUniform("bloomSaturation", bloomSaturation);
        bloomCombine.setUniform("baseSaturation", baseSaturation);
        bloomCombine.setUniform("baseTexture", baseTexture.getTexture());
        bloomCombine.setUniform("bloomTexture", bloomExtractTexture.getTexture());

        // Finally the blurred and saturated texture to the screen
        GameRoot::instance().renderWindow.draw(bloomExtractSprite, &bloomCombine);
    }
    else
    {
        const sf::Sprite baseTextureSprite {baseTexture.getTexture()};
        GameRoot::instance().renderWindow.draw(baseTextureSprite);
    }
}


void Bloom::setGaussianBlurParameters(const float dx, const float dy)
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


float Bloom::computeGaussian(const float n) const
{
    const float sigma = blurAmount;

    // Gaussian function
    const float x = 1.0f / std::sqrt(2 * std::numbers::pi * sigma);
    const float y = std::exp(-(n * n) / (2 * (sigma * sigma)));
    return x * y;
}

