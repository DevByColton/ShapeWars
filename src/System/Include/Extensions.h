#ifndef EXTENSIONS_H
#define EXTENSIONS_H

#include <cmath>
#include <string>
#include "Quaternion.h"
#include "SFML/System/Vector2.hpp"
#include "SFML/System/Vector3.hpp"


static constexpr float PI = 3.14159265359f;
static constexpr float TWO_PI = PI * 2.f;
static constexpr float FORTY_FIVE_DEGREES = PI / 4.f;

struct Extensions
{
    static float toAngle(const sf::Vector2f &vector);
    static sf::Vector2f fromPolar(float angle, float magnitude);
    static float distanceSquared(const sf::Vector2f &vector1, const sf::Vector2f &vector2);
    static float distance(const sf::Vector2f &vector1, const sf::Vector2f &vector2);
    static sf::Vector2f multiply(const sf::Vector2f &vector1, const sf::Vector2f &vector2);
    static sf::Vector3f multiply(const sf::Vector3f &vector1, const sf::Vector3f &vector2);
    static sf::Vector2f transform(const sf::Vector2f &vector, const Quaternion &rotation);
    static float normalize(float value, float min, float max);
    static std::string formatNumberWithCommas(int number);
};


#endif //EXTENSIONS_H
