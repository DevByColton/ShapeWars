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
    std::uniform_real_distribution<float> randomLerpValue {0.f, 1.f};
    std::uniform_real_distribution<float> randomHue {0.f, 6.f};
    std::uniform_real_distribution<float> randomHueShifted {0.f, 2.f};
};



#endif //COLORPICKER_H
