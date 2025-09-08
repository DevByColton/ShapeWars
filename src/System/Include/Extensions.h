#ifndef EXTENSIONS_H
#define EXTENSIONS_H

#include <cmath>
#include <string>
#include "Quaternion.h"
#include "SFML/System/Vector2.hpp"
#include "SFML/System/Vector3.hpp"

static float toAngle(const sf::Vector2f &vector) {
    return std::atan2(vector.y, vector.x);
}


static sf::Vector2f fromPolar(const float angle, const float magnitude) {
    const float cos = std::cos(angle);
    const float sin = std::sin(angle);
    return magnitude * sf::Vector2f(cos, sin);
}


static float distanceSquared(const sf::Vector2f vector1, const sf::Vector2f vector2) {
    const float distanceX = vector1.x - vector2.x;
    const float distanceY = vector1.y - vector2.y;
    return  distanceX * distanceX + distanceY * distanceY;
}

static sf::Vector2f multiply(const sf::Vector2f vector1, const sf::Vector2f vector2) {
    float x = vector1.x * vector2.x;
    float y = vector1.y * vector2.y;
    return {x, y};
}


static sf::Vector3f multiply(const sf::Vector3f vector1, const sf::Vector3f vector2) {
    float x = vector1.x * vector2.x;
    float y = vector1.y * vector2.y;
    float z = vector1.z * vector2.z;
    return {x, y, z};
}


static sf::Vector2f transform(const sf::Vector2f vector, const Quaternion rotation) {
    const sf::Vector3f vector3_1 = {rotation.x + rotation.x, rotation.y + rotation.y, rotation.z + rotation.z};
    const sf::Vector3f vector3_2 = {rotation.x, rotation.x, rotation.w};
    const sf::Vector3f vector3_3 = {1.0f, rotation.y, rotation.z};
    const sf::Vector3f vector3_4 = multiply(vector3_1, vector3_2);
    const sf::Vector3f vector3_5 = multiply(vector3_1, vector3_3);

    auto x = static_cast<float>(static_cast<double>(vector.x) * (1.0 - static_cast<double>(vector3_5.y) - static_cast<double>(vector3_5.z)) + static_cast<double>(vector.y) * (static_cast<double>(vector3_4.y) - static_cast<double>(vector3_4.z)));
    auto y = static_cast<float>(static_cast<double>(vector.x) * (static_cast<double>(vector3_4.y) + static_cast<double>(vector3_4.z)) + static_cast<double>(vector.y) * (1.0 - static_cast<double>(vector3_4.x) - static_cast<double>(vector3_5.z)));

    return {x, y};
}


static float normalize(const float value, const float min, const float max) {
    return (value - min) / (max - min);
}


static std::string formatNumberWithCommas(const int number) {

    std::string numberString = std::to_string(number);
    int first_digit_index = 0;

    // Handle negative numbers, start after the - sign
    if (number < 0)
        first_digit_index = 1;

    // Insert commas from right to left, every 3 characters
    for (int i = numberString.length() - 3; i > first_digit_index; i -= 3)
        numberString.insert(i, ",");

    return numberString;
}


#endif //EXTENSIONS_H
