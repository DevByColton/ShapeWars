#ifndef FLOATINGKILLTEXTS_H
#define FLOATINGKILLTEXTS_H
#include <array>
#include "../../Content/Include/Art.h"
#include "SFML/Graphics/Text.hpp"


class FloatingKillTexts {
private:
    class FloatingKillText
    {
    private:
        union
        {
            struct
            {
                float fadeTime;
                float fadeScale;
            };

            FloatingKillText *next {nullptr};
        };

        const float baseFadeTime = 1.0f;
        const float baseFadeScale = 1.0f;
        sf::Text text {Art::instance().font, {}, 16};

    public:
        bool isActive = false;

        FloatingKillText *getNext() const;
        void setNext(FloatingKillText *);
        void activate(int, sf::Vector2f);
        bool fadeIn();
        void reset();
        void draw() const;
    };

    static constexpr int MAX_KILL_TEXT_COUNT = 250;
    FloatingKillText *firstAvailable {nullptr};

public:
    FloatingKillTexts();

    static FloatingKillTexts &instance()
    {
        static auto *instance = new FloatingKillTexts;
        return *instance;
    }

    std::array<FloatingKillText, MAX_KILL_TEXT_COUNT> floatingKillTexts {};

    void add(int, sf::Vector2f);
    void update();
    void draw() const;
};



#endif //FLOATINGKILLTEXTS_H
