#ifndef COLORPICKER_H
#define COLORPICKER_H
#include <random>
#include "SFML/Graphics/Color.hpp"


struct ColorPicker {
    static ColorPicker &instance()
    {
        static auto *instance = new ColorPicker;
        return *instance;
    }

    float generateHue();
    float generateShiftedHue(float);
    sf::Color hsvToRgb(float, float, float);
    sf::Color lerp(sf::Color, sf::Color);

    std::default_random_engine randEngine {std::random_device{}()};
    std::uniform_real_distribution<float> randomLerpValue {0.0f, 1.0f};
    std::uniform_real_distribution<float> randomHue {0.0f, 6.0f};
    std::uniform_real_distribution<float> randomHueShifted {0.0f, 2.0f};
};



#endif //COLORPICKER_H
