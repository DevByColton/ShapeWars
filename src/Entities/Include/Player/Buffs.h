#ifndef BUFFS_H
#define BUFFS_H
#include <random>
#include "../../../Content/Include/Art.h"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Graphics/RenderTexture.hpp"


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
        sf::Text timerText {Art::instance().turretRoadFont, {}, 28};

        bool isBeingUsed() const;
        void setUsing();
        void use();
        void reset();
        void activate(BuffType buffType);
        void update();
    };

    static constexpr int DROP_RATE = 7;
    static constexpr float POTENTIAL_BUFF_DROP_INTERVAL = 1.f;
    float timeUntilPotentialBuffDrop = POTENTIAL_BUFF_DROP_INTERVAL;
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
    void drawBuffs(sf::RenderTexture& renderTexture);
    void drawBuffDrops() const;
    void drawText(sf::RenderTexture& renderTexture) const;
};



#endif //BUFFS_H
