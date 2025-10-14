#ifndef DIRECTIONALATTACK_H
#define DIRECTIONALATTACK_H
#include "ShapeKeeperBodyPart.h"


struct DirectionalAttack {
    static constexpr float DIRECTIONAL_ATTACK_DURATION = 3.f;
    float directionalAttackTime = 0.f;
    bool isDoingDirectionalAttack = false;

    void update(ShapeKeeperBodyPart* bodyPart);
};



#endif //DIRECTIONALATTACK_H
