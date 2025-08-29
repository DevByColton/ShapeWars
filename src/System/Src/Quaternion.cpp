#include <cmath>
#include "../Include/Quaternion.h"
#include "../Include/Extensions.h"
#include "SFML/System/Vector3.hpp"

Quaternion::Quaternion(const float x, const float y, const float z, const float w) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}


Quaternion Quaternion::createFromYawPitchRoll(const float yaw, const float pitch, const float roll) {

    const double x1 = static_cast<double>(roll) * 0.5;
    const float x2 = pitch * 0.5f;
    const float x3 = yaw * 0.5f;
    const float num1 = std::sin(static_cast<float>(x1));
    const float num2 = std::cos(static_cast<float>(x1));
    const float num3 = std::sin(x2);
    const float num4 = std::cos(x2);
    const float num5 = std::sin(x3);
    const float num6 = std::cos(x3);

    const auto x = static_cast<float>(static_cast<double>(num6) * static_cast<double>(num3) * static_cast<double>(num2) + static_cast<double>(num5) * static_cast<double>(num4) * static_cast<double>(num1));
    const auto y = static_cast<float>(static_cast<double>(num5) * static_cast<double>(num4) * static_cast<double>(num2) - static_cast<double>(num6) * static_cast<double>(num3) * static_cast<double>(num1));
    const auto z = static_cast<float>(static_cast<double>(num6) * static_cast<double>(num4) * static_cast<double>(num1) - static_cast<double>(num5) * static_cast<double>(num3) * static_cast<double>(num2));
    const auto w = static_cast<float>(static_cast<double>(num6) * static_cast<double>(num4) * static_cast<double>(num2) + static_cast<double>(num5) * static_cast<double>(num3) * static_cast<double>(num1));

    return {x, y, z, w};

}
