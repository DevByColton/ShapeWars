#ifndef SHAPEKEEPER_H
#define SHAPEKEEPER_H
#include "LasersBeams.h"
#include "ShapeKeeperBodyPart.h"
#include "ShapeKeeperCore.h"
#include "ShapeKeeperHealthContainer.h"
#include "../../../Content/Include/Art.h"


class ShapeKeeper {
private:
    ShapeKeeperHealthContainer healthContainer {};

public:
    static ShapeKeeper &instance()
    {
        static auto *instance = new ShapeKeeper;
        return *instance;
    }

    ShapeKeeperCore core {
        Art::instance().shapeKeeperCore,
        {255, 51, 113, 255},
        &healthContainer.core
    };

    ShapeKeeperBodyPart top {
        Art::instance().shapeKeeperTop,
        Art::instance().shapeKeeperTopTrail,
        {255, 233, 38, 255},
        &healthContainer.top,
        &core,
        -8.f,
        -167.f
    };
    ShapeKeeperBodyPart middleLeft {
        Art::instance().shapeKeeperMiddleLeft,
        Art::instance().shapeKeeperMiddleLeftTrail,
        {53, 255, 203, 255},
        &healthContainer.middleLeft,
        &core,
        -167.f,
        -49.f
    };

    ShapeKeeperBodyPart middleRight {
        Art::instance().shapeKeeperMiddleRight,
        Art::instance().shapeKeeperMiddleRightTrail,
        {48, 255, 53, 255},
        &healthContainer.middleRight,
        &core,
        154.f,
        -49.f
    };

    ShapeKeeperBodyPart bottomLeft {
        Art::instance().shapeKeeperBottomLeft,
        Art::instance().shapeKeeperBottomLeftTrail,
        {255, 156, 25, 255},
        &healthContainer.bottomLeft,
        &core,
        -105.f,
        133.f
    };

    ShapeKeeperBodyPart bottomRight {
        Art::instance().shapeKeeperBottomRight,
        Art::instance().shapeKeeperBottomRightTrail,
        {241, 38, 255, 255},
        &healthContainer.bottomRight,
        &core,
        93.f,
        133.f
    };

    LasersBeams lasersAttack {&core};

    bool isActive = false;

    void startEncounter();
    void endEncounter();
    bool canTakeCoreDamage() const;
    void update();
    void draw();
    void drawText();
};



#endif //SHAPEKEEPER_H
