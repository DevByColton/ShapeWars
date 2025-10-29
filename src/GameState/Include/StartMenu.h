#ifndef STARTMENU_H
#define STARTMENU_H
#include <random>
#include "IGameState.h"
#include "../../GameRoot.h"


struct StartMenu final : IGameState
{
    static StartMenu &instance()
    {
        static auto *instance = new StartMenu;
        return *instance;
    }

    static constexpr float TIME_UNTIL_NEW_EXPLOSION_DURATION = 0.08f;
    float timeUntilNewExplosionElapsed = TIME_UNTIL_NEW_EXPLOSION_DURATION;
    std::default_random_engine particleDurationRandEngine {std::random_device{}()};
    std::default_random_engine widthRandEngine {std::random_device{}()};
    std::default_random_engine heightRandEngine {std::random_device{}()};
    std::uniform_real_distribution<float> particleDurationDistribution {0.8f, 1.2f};
    std::uniform_real_distribution<float> widthDistribution {20.f, GameRoot::instance().windowSizeF.x - 20.f};
    std::uniform_real_distribution<float> heightDistribution {20.f, GameRoot::instance().windowSizeF.y - 20.f};

    void processKeyReleased(const sf::Event::KeyReleased* keyReleased) override;
    void processJoystickButtonReleased(const sf::Event::JoystickButtonReleased* joystickButtonReleased) override;
    void processJoystickAxisMoved(const sf::Event::JoystickMoved* joystickMoved) override;
    void update() override;
    void renderGaussianBlur() override;
    void renderToScreen() override;
};



#endif //STARTMENU_H
