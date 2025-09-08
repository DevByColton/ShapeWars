#include "../Include/ColorPicker.h"
#include <assert.h>
#include <cmath>

float ColorPicker::generateHue()
{
    return randomHue(randEngine);
}


float ColorPicker::generateShiftedHue(const float fromHue)
{
    return std::fmod(fromHue + randomHueShifted(randEngine), 6.0f);
}


sf::Color ColorPicker::hsvToRgb(const float hue, const float saturation, const float value)
{
    if (hue == 0.0f || saturation == 0.0f)
        return sf::Color::White;

    const float c = saturation * value;
    const float x = c * (1 - std::abs(std::fmod(hue, 2) - 1.0f));
    const float m = value - c;

    if (hue < 1.0f)
        return sf::Color((c + m) * 255, (x + m) * 255, m * 255, 255);

    if (hue < 2.0f)
        return sf::Color((x + m) * 255, (c + m) * 255, m * 255, 255);

    if (hue < 3.0f)
        return sf::Color(m * 255, (c + m) * 255, (x + m) * 255, 255);

    if (hue < 4.0f)
        return sf::Color(m * 255, (x + m) * 255, (c + m) * 255, 255);

    if (hue < 5.0)
        return sf::Color((x + m) * 255, m * 255, (c + m) * 255, 255);

    if (hue < 6.0)
        return sf::Color((c + m) * 255, m * 255, (x + m) * 255, 255);

    return sf::Color::White;
}


sf::Color ColorPicker::lerp(const sf::Color startColor, const sf::Color endColor)
{
    float lerpValue = randomLerpValue(randEngine);
    sf::Color color = startColor + (endColor - startColor);

    return sf::Color(
        static_cast<std::uint8_t>(color.r * lerpValue),
        static_cast<std::uint8_t>(color.g * lerpValue),
        static_cast<std::uint8_t>(color.b * lerpValue),
        static_cast<std::uint8_t>(color.a * lerpValue)
    );
}

