#ifndef GRID_H
#define GRID_H
#include <vector>
#include "../Content/Include/Art.h"
#include "SFML/Graphics/Color.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/System/Vector3.hpp"


class Grid {
private:
    struct PointMass
    {
        PointMass();
        PointMass(float inverseMass, const sf::Vector2f &position);

        static constexpr float DEFAULT_DAMPING = 0.98f;
        float inverseMass;
        float damping;
        sf::Vector2f position;
        sf::Vector2f velocity;
        sf::Vector2f acceleration;

        void applyForce(sf::Vector2f);
        void increaseDamping(float);
        void update();
    };

    struct Spring
    {
        Spring();
        Spring(PointMass*, PointMass*, float, float);

        float targetLength;
        float stiffness;
        float damping;
        PointMass *point1;
        PointMass *point2;

        void update() const;
    };

    // NOTE: There are a number of values to tweak in the creation of the grid. The most important ones are
    //       the stiffness and damping of the springs. The stiffness and damping of the border anchors and interior
    //       anchors are set independently of the main springs. Higher stiffness values will make the springs
    //       oscillate more quickly, and higher damping values will cause the springs to slow down faster
    static constexpr float STIFFNESS {0.1f};
    static constexpr float DAMPING {0.2f};
    static constexpr int NUM_GRID_POINTS = 1'600;
    float spacing = 0.f;
    std::size_t rowCount {0};
    std::size_t columnCount {0};
    std::vector<std::vector<PointMass>> fixedPoints;
    std::vector<std::vector<PointMass>> points;
    std::vector<Spring> springs = std::vector<Spring>(0);
    const sf::Color gridColor {85, 0, 255, 115};

public:
    Grid();

    static Grid &instance()
    {
        static auto *instance = new Grid;
        return *instance;
    }

    void applyImplosiveForce(sf::Vector2f, float, float);
    void applyExplosiveForce(sf::Vector2f, float, float, float);
    void update();
    void draw();
};



#endif //GRID_H
