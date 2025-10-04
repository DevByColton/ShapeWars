#ifndef BUFFS_H
#define BUFFS_H
#include "../../Content/Include/Art.h"
#include "SFML/Graphics/Sprite.hpp"


class Buffs {
private:
    struct Buff
    {
        bool isActive = false;
        sf::Sprite buffFrame {Art::instance().buffFrame};
        sf::Sprite buffIcon {Art::instance().buffIconPlaceHolder};
    };

    Buff buff1 {};
    Buff buff2 {};
    Buff buff3 {};
    Buff buff4 {};

public:
    Buffs();

    static Buffs &instance()
    {
        static auto *instance = new Buffs;
        return *instance;
    }

    void draw();
};



#endif //BUFFS_H
