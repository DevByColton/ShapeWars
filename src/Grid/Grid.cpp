#include <cmath>
#include "Grid.h"
#include "../GameRoot.h"
#include "../Content/Include/GaussianBlur.h"
#include "../System/Include/Extensions.h"


Grid::Grid()
{
    const float width = GameRoot::instance().windowRectangle.size.x + 50.f;
    const float height = GameRoot::instance().windowRectangle.size.y + 50.f;
    spacing = std::sqrt(width * height / NUM_GRID_POINTS);
    columnCount = static_cast<std::size_t>(width / spacing + 1.f);
    rowCount = static_cast<std::size_t>(height / spacing + 1.f);

    fixedPoints = {columnCount, std::vector<PointMass>(rowCount)};
    points = {columnCount, std::vector<PointMass>(rowCount)};

    // Set up the initial point mass positions, and fixed anchor points masses
    for (float x = 0.f, col = 0; x <= width; x += spacing, col += 1)
        for (float y = 0.f, row = 0; y <= height; y += spacing, row += 1)
        {
            fixedPoints.at(col).at(row) = {0.f, {x, y}};
            points.at(col).at(row) = {1.f, {x, y}};
        }

    // Link the point masses with the springs
    for (std::size_t col = 0; col < columnCount; col++)
        for (std::size_t row = 0; row < rowCount; row++)
        {
            // Anchor the border points
            if (col == 0 || row == 0 || col == columnCount - 1 || row == rowCount - 1)
                springs.push_back({
                    &fixedPoints.at(col).at(row),
                    &points.at(col).at(row),
                    0.1f,
                    0.1f
                });
            else if (col % 3 == 0 && row % 3 == 0)
                springs.push_back({
                    &fixedPoints.at(col).at(row),
                    &points.at(col).at(row),
                    0.002f,
                    0.02f
                });

            // Inner springs
            if (col > 0)
                springs.push_back({
                    &points.at(col - 1).at(row),
                    &points.at(col).at(row),
                    STIFFNESS,
                    DAMPING
                });
            if (row > 0)
                springs.push_back({
                    &points.at(col).at(row - 1),
                    &points.at(col).at(row),
                    STIFFNESS,
                    DAMPING
                });
        }

    // Make sure the springs and points are only as big as they need to be
    fixedPoints.shrink_to_fit();
    points.shrink_to_fit();
    for (std::size_t i = 0; i < rowCount; i++)
    {
        fixedPoints.at(i).shrink_to_fit();
        points.at(i).shrink_to_fit();
    }

    springs.shrink_to_fit();
}


Grid::PointMass::PointMass()
{
    inverseMass = 0.f;
    damping = DEFAULT_DAMPING;
    position = {0.0, 0.0};
    velocity = {0.0, 0.0};
    acceleration = {0.0, 0.0};
}

Grid::PointMass::PointMass(const float inverseMass, const sf::Vector2f &position)
{
    this->inverseMass = inverseMass;
    damping = DEFAULT_DAMPING;
    this->position = position;
    velocity = {0.0, 0.0};
    acceleration = {0.0, 0.0};
}


Grid::Spring::Spring()
{
    targetLength = 0.f;
    stiffness = 0.f;
    damping = 0.f;
    point1 = {nullptr};
    point2 = {nullptr};
}


Grid::Spring::Spring(PointMass *point1, PointMass *point2, const float stiffness, const float damping)
: point1(point1), point2(point2)
{
    this->stiffness = stiffness;
    this->damping = damping;

    // Set the length slightly less than the distance between the two points
    targetLength = Extensions::distance(point1->position, point2->position) * 0.95f;
}


void Grid::PointMass::applyForce(const sf::Vector2f force)
{
    acceleration += force * inverseMass;
}


void Grid::PointMass::increaseDamping(const float factor)
{
    damping *= factor;
}


void Grid::applyImplosiveForce(const sf::Vector2f position, const float force, const float radius)
{
    const float radii = radius * radius;

    for (std::size_t col = 0; col < columnCount; col++)
        for (std::size_t row = 0; row < rowCount; row++)
        {
            const float distSquared = Extensions::distanceSquared(position, points.at(col).at(row).position);

            if (distSquared < radii)
            {
                const sf::Vector2f direction = position - points.at(col).at(row).position;
                points.at(col).at(row).applyForce(10.f * force * direction / (100.f + distSquared));
                points.at(col).at(row).increaseDamping(0.6f);
            }
        }
}


void Grid::applyExplosiveForce(const sf::Vector2f position, const float force, const float radius, const float damping)
{
    const float radii = radius * radius;

    for (std::size_t col = 0; col < columnCount; col++)
        for (std::size_t row = 0; row < rowCount; row++)
        {
            const float distSquared = Extensions::distanceSquared(position, points.at(col).at(row).position);

            if (distSquared < radii)
            {
                const sf::Vector2f direction = points.at(col).at(row).position - position;
                points.at(col).at(row).applyForce(100 * force * direction / (10'000.f + distSquared));
                points.at(col).at(row).increaseDamping(damping);
            }
        }
}


void Grid::PointMass::update()
{
    velocity += acceleration;
    position += velocity;
    acceleration = {0.0, 0.0};

    if (velocity.lengthSquared() < 0.0001f)
        velocity = {0.0, 0.0};

    velocity *= damping;
    damping = 0.98f;
}


/**
* A spring connects two point masses, and, if stretched past its natural length,
* applies a force pulling the masses together. Springs follow a modified version
* of Hooke's Law with damping
*
* f = -kx - bv
*
* f is the force produced by the spring
* k is the spring constant, or the stiffness of the spring
* x is the distance the spring is stretched beyond its natural length
* b is the damping factor
* v is the velocity
*/
void Grid::Spring::update() const
{
    sf::Vector2f x = point1->position - point2->position;
    const float length = x.length();

    if (length <= targetLength)
        return;

    x = x / length * (length - targetLength);
    const sf::Vector2f v = point2->velocity - point1->velocity;
    const sf::Vector2f force = stiffness * x - v * damping;

    point1->applyForce(-force);
    point2->applyForce(force);
}


void Grid::update()
{
    for (std::size_t i = 0; i < springs.size(); i++)
        springs.at(i).update();

    for (std::size_t col = 0; col < columnCount; col++)
        for (std::size_t row = 0; row < rowCount; row++)
            points.at(col).at(row).update();
}


void Grid::draw()
{
    for (std::size_t col = 0; col < columnCount; col++)
    {
        std::vector<sf::Vertex> vertices {0};

        for (std::size_t row = 0; row < rowCount; row++)
        {
            const sf::Vertex currentPoint = {{points.at(col).at(row).position.x, points.at(col).at(row).position.y}, gridColor};

            if (col >= 1)
            {
                // Draw horizontal line
                const sf::Vertex horizontal = {{points.at(col - 1).at(row).position.x, points.at(col - 1).at(row).position.y}, gridColor};
                vertices.push_back(currentPoint);
                vertices.push_back(horizontal);

                // Draw horizontal mid-line
                if (row != 0)
                {
                    const float topLeft = points.at(col - 1).at(row - 1).position.y;
                    const float topRight = points.at(col).at(row - 1).position.y;

                    sf::Vertex midLeft = {{horizontal.position.x, (horizontal.position.y + topLeft) / 2.f}, gridColor};
                    sf::Vertex midRight = {{currentPoint.position.x, (currentPoint.position.y + topRight) / 2.f}, gridColor};
                    vertices.push_back(midLeft);
                    vertices.push_back(midRight);
                }
            }

            if (row >= 1)
            {
                // Draw vertical line
                const sf::Vertex vertical = {{points.at(col).at(row - 1).position.x, points.at(col).at(row - 1).position.y}, gridColor};
                vertices.push_back(currentPoint);
                vertices.push_back(vertical);

                // Draw vertical mid-line
                if (col != 0)
                {
                    const float topLeft = points.at(col - 1).at(row - 1).position.x;
                    const float bottomLeft = points.at(col - 1).at(row).position.x;

                    sf::Vertex midTop = {{(vertical.position.x + topLeft) / 2.f, vertical.position.y}, gridColor};
                    sf::Vertex midBottom = {{(currentPoint.position.x + bottomLeft) / 2.f, currentPoint.position.y}, gridColor};
                    vertices.push_back(midTop);
                    vertices.push_back(midBottom);
                }
            }
        }

        GaussianBlur::instance().drawToBase(vertices);
    }
}

