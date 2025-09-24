#include <numbers>
#include <cmath>
#include "../Include/RandomVector.h"
#include "../Include/Extensions.h"


sf::Vector2f RandomVector::next(const float min, const float max)
{
    std::uniform_real_distribution<float> nextFloat {min, max};

    const float theta = nextFloatZeroAndOne(randEngine) * 2 * PI;
    const float length = nextFloat(randEngine);
    const float x = length * std::cos(theta);
    const float y = length * std::sin(theta);
    return {x, y};
}
