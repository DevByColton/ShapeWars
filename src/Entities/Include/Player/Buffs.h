#ifndef BUFFS_H
#define BUFFS_H
#include <random>
#include "../../../Content/Include/Art.h"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Text.hpp"


enum struct BuffType
{
    None,
    BulletsAllDirections,
    Shotgun,
    Invincible,
    Boosters
};

class Buffs {
private:
    struct BuffDrop
    {
        bool isActive = false;
        bool isPickedUp = false;
        float radius = 15.f;
        BuffType type = BuffType::None;
        sf::Sprite icon {Art::instance().buffIconPlaceHolder};

        bool availableForPickUp() const;
        sf::Vector2f getPosition() const;
        void reset();
        void activate(BuffType buffType, sf::Vector2f position);
        void update();
        void draw() const;
    };

    struct Buff
    {
        const float maxBuffDuration = 10.9f;
        float duration = 0.f;
        bool isActive = false;
        BuffType type = BuffType::None;
        sf::Sprite frame {Art::instance().buffFrame};
        sf::Sprite icon {Art::instance().buffIconPlaceHolder};
        sf::Text timerText {Art::instance().font, {}, 28};

        bool isBeingUsed() const;
        void setUsing();
        void use();
        void reset();
        void activate(BuffType buffType);
        void update();
    };

    const int dropRate = 8;
    const float potentialBuffDropInterval = 1.f;
    float timeUntilPotentialBuffDrop = potentialBuffDropInterval;
    bool allowBuffDropChance = false;
    std::default_random_engine randEngine {std::random_device{}()};
    std::uniform_int_distribution<int> buffDropDistribution {1, 100};
    std::uniform_int_distribution<int> buffTypeDistribution {1, 4};
    Buff buff1 {};
    Buff buff2 {};
    Buff buff3 {};
    Buff buff4 {};

    BuffDrop* nextAvailableBuffDrop();
    Buff* nextAvailableBuff();

public:
    Buffs();

    static Buffs &instance()
    {
        static auto *instance = new Buffs;
        return *instance;
    }

    BuffDrop buffDrop1 {};
    BuffDrop buffDrop2 {};
    BuffDrop buffDrop3 {};

    bool canPickUpBuff();
    void checkBuffDrop(const sf::Vector2f& position);
    void useBuff(int buffNumber);
    void resetBuffDrops();
    void resetBuffs();
    void update();
    void draw();
    void drawText() const;
};



#endif //BUFFS_H
