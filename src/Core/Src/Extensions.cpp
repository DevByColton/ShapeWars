#include "../Include/Extensions.h"
#include <algorithm>
#include "SFML/System/Vector2.hpp"
#include <cmath>
#include <string>
#include "SFML/System/Vector3.hpp"
#include "../Include/Quaternion.h"


float Extensions::toAngle(const sf::Vector2f &vector) {
    return std::atan2(vector.y, vector.x);
}


sf::Vector2f Extensions::fromPolar(const float angle, const float magnitude) {
    const float cos = std::cos(angle);
    const float sin = std::sin(angle);
    return magnitude * sf::Vector2f(cos, sin);
}


float Extensions::distanceSquared(const sf::Vector2f &vector1, const sf::Vector2f &vector2) {
    const float distanceX = vector1.x - vector2.x;
    const float distanceY = vector1.y - vector2.y;
    return distanceX * distanceX + distanceY * distanceY;
}


float Extensions::distance(const sf::Vector2f& vector1, const sf::Vector2f& vector2)
{
    const float result = distanceSquared(vector1, vector2);
    return std::sqrt(result);
}


sf::Vector2f Extensions::multiply(const sf::Vector2f &vector1, const sf::Vector2f &vector2) {
    float x = vector1.x * vector2.x;
    float y = vector1.y * vector2.y;
    return {x, y};
}


sf::Vector3f Extensions::multiply(const sf::Vector3f &vector1, const sf::Vector3f &vector2) {
    float x = vector1.x * vector2.x;
    float y = vector1.y * vector2.y;
    float z = vector1.z * vector2.z;
    return {x, y, z};
}


sf::Vector2f Extensions::transform(const sf::Vector2f &vector, const Quaternion &rotation) {
    const sf::Vector3f vector3_1 = {rotation.x + rotation.x, rotation.y + rotation.y, rotation.z + rotation.z};
    const sf::Vector3f vector3_2 = {rotation.x, rotation.x, rotation.w};
    const sf::Vector3f vector3_3 = {1.f, rotation.y, rotation.z};
    const sf::Vector3f vector3_4 = multiply(vector3_1, vector3_2);
    const sf::Vector3f vector3_5 = multiply(vector3_1, vector3_3);

    auto x = static_cast<float>(static_cast<double>(vector.x) * (1.0 - static_cast<double>(vector3_5.y) - static_cast<double>(vector3_5.z)) + static_cast<double>(vector.y) * (static_cast<double>(vector3_4.y) - static_cast<double>(vector3_4.z)));
    auto y = static_cast<float>(static_cast<double>(vector.x) * (static_cast<double>(vector3_4.y) + static_cast<double>(vector3_4.z)) + static_cast<double>(vector.y) * (1.0 - static_cast<double>(vector3_4.x) - static_cast<double>(vector3_5.z)));

    return {x, y};
}


std::string Extensions::formatNumberWithCommas(const int number) {

    std::string numberString = std::to_string(number);
    int firstDigitIndex = 0;

    // Handle negative numbers, start after the - sign
    if (number < 0)
        firstDigitIndex = 1;

    // Insert commas from right to left, every 3 characters
    for (int i = numberString.length() - 3; i > firstDigitIndex; i -= 3)
        numberString.insert(i, ",");

    return numberString;
}


void Extensions::clamp(sf::Vector2f& vector, const float min, const float max)
{
    vector.x = std::clamp(vector.x, min, max);
    vector.y = std::clamp(vector.y, min, max);
}


sf::Vector2f Extensions::lerp(const sf::Vector2f& start, const sf::Vector2f& end, const float t)
{
    return {
        std::lerp(start.x, end.x, t),
        std::lerp(start.y, end.y, t)
    };
}


sf::Vector2f Extensions::easeBackIn(const sf::Vector2f& start, const sf::Vector2f& end, const float t)
{
    constexpr float c1 = 1.70158f;
    constexpr float c3 = c1 + 1.f;
    const float easeBackIn = c3 * t * t * t - c1 * t * t;

    return {
        std::lerp(start.x, end.x, easeBackIn),
        std::lerp(start.y, end.y, easeBackIn)
    };
}


sf::Vector2f Extensions::easeOutBack(const sf::Vector2f& start, const sf::Vector2f& end, const float t)
{
    constexpr float c1 = 1.70158f;
    constexpr float c3 = c1 + 1.f;
    const float easeOutBack = 1.f + c3 * std::pow(t - 1.f, 3) + c1 * std::pow(t - 1.f, 2);

    return {
        std::lerp(start.x, end.x, easeOutBack),
        std::lerp(start.y, end.y, easeOutBack)
    };
}


sf::Vector2f Extensions::easeInOutElastic(const sf::Vector2f& start, const sf::Vector2f& end, const float t)
{
    constexpr float c5 = 2.f * PI / 4.5;
    float easeInOutElastic = 0.f;

    if (t == 0.f)
        easeInOutElastic = 0.f;
    else if (t == 1.f)
        easeInOutElastic = 1.f;
    else if (t < 0.5f)
        easeInOutElastic = -(std::pow(2, 20 * t - 10) * std::sin((20 * t - 11.125) * c5)) / 2.f;
    else
        easeInOutElastic = (std::pow(2, -20 * t + 10) * std::sin((20 * t - 11.125) * c5)) / 2.f + 1.f;

    return {
        std::lerp(start.x, end.x, easeInOutElastic),
        std::lerp(start.y, end.y, easeInOutElastic)
    };
}


sf::Vector2f Extensions::easeInOutBack(const sf::Vector2f& start, const sf::Vector2f& end, float t)
{
    constexpr float c1 = 1.70158f;
    constexpr float c2 = c1 * 1.525f;
    float easeInOutBack = 0.f;

    if (t < 0.5)
        easeInOutBack = std::pow(2 * t, 2) * ((c2 + 1.f) * 2.f * t - c2) / 2.f;
    else
        easeInOutBack = (std::pow(2 * t - 2, 2) * ((c2 + 1.f) * (t * 2.f - 2.f) + c2) + 2.f) / 2.f;

    return {
        std::lerp(start.x, end.x, easeInOutBack),
        std::lerp(start.y, end.y, easeInOutBack)
    };
}


sf::Vector2f Extensions::easeInCubic(const sf::Vector2f& start, const sf::Vector2f& end, const float t)
{
    const float easeInCubic = t * t * t;

    return {
        std::lerp(start.x, end.x, easeInCubic),
        std::lerp(start.y, end.y, easeInCubic)
    };
}


sf::Vector2f Extensions::easeOutCubic(const sf::Vector2f& start, const sf::Vector2f& end, const float t)
{
    const float easeOutSine = 1.f - std::pow(1.f - t, 3);

    return {
        std::lerp(start.x, end.x, easeOutSine),
        std::lerp(start.y, end.y, easeOutSine)
    };
}


float Extensions::easeInOutSine(const float start, const float end, const float t)
{
    const float easeInOutSine = -(std::cos(PI * t) - 1.f) / 2.f;

    return std::lerp(start, end, easeInOutSine);
}