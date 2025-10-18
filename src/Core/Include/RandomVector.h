#ifndef RANDOMVECTOR_H
#define RANDOMVECTOR_H
#include <random>
#include "SFML/System/Vector2.hpp"


struct RandomVector {

    static RandomVector &instance()
    {
        static auto *instance = new RandomVector;
        return *instance;
    }

    std::default_random_engine randEngine {std::random_device{}()};
    std::uniform_real_distribution<float> nextFloatZeroAndOne {0.f, 1.f};

    sf::Vector2f next(float, float);
};



#endif //RANDOMVECTOR_H
