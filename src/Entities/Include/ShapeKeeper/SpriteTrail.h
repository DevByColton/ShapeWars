#ifndef SPRITETRAIL_H
#define SPRITETRAIL_H
#include <array>
#include "../../../Content/Include/GaussianBlur.h"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/System/Vector2.hpp"


class SpriteTrail {
private:
    struct Trail
    {
        static constexpr float DURATION = 0.65f;
        static constexpr std::uint8_t MAX_ALPHA = 50;
        float elapsed = 0.f;
        bool isActive = false;
        std::uint8_t alpha = MAX_ALPHA;
        sf::Vector2f position {0.f, 0.f};

        void reset();
        void activate(const sf::Vector2f& position);
    };

    static constexpr int SIZE = 8;
    static constexpr float CREATE_NEW_TRAIL_DURATION = 0.1f;
    float createNewTrailTime = CREATE_NEW_TRAIL_DURATION;
    sf::Sprite* spriteToTrail {nullptr};
    std::array<Trail, SIZE> trail;

public:
    explicit SpriteTrail(sf::Sprite* spriteToTrail);

    void reset();
    void update();
    void draw();
};



#endif //SPRITETRAIL_H
