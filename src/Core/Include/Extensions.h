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
    static std::string formatNumberWithCommas(int number);
    static void clamp(sf::Vector2f &vector, float min, float max);
    static sf::Vector2f lerp(const sf::Vector2f& start, const sf::Vector2f& end, float t);
    static sf::Vector2f easeBackIn(const sf::Vector2f& start, const sf::Vector2f& end, float t);
    static sf::Vector2f easeOutBack(const sf::Vector2f& start, const sf::Vector2f& end, float t);
    static sf::Vector2f easeInOutElastic(const sf::Vector2f& start, const sf::Vector2f& end, float t);
    static sf::Vector2f easeInOutBack(const sf::Vector2f& start, const sf::Vector2f& end, float t);
    static float easeInOutSine(float start, float end, float t);
};


#endif //EXTENSIONS_H
