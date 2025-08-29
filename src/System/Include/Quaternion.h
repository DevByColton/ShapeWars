#ifndef QUATERNION_H
#define QUATERNION_H
#include "SFML/System/Vector2.hpp"


struct Quaternion {
    Quaternion() = default;
    Quaternion(float, float, float, float);
    ~Quaternion() = default;

    float x = 0.0;
    float y = 0.0;
    float z = 0.0;
    float w = 1.0;

    static Quaternion createFromYawPitchRoll(float, float, float);
};



#endif //QUATERNION_H
