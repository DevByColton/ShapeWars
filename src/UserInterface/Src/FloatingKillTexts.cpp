#include "../Include/FloatingKillTexts.h"
#include "../../GameRoot.h"


FloatingKillTexts::FloatingKillTexts()
{
    // Set the head of the enemy kill texts
    firstAvailable = &floatingKillTexts.at(0);

    // Link them up
    for (int i = 0; i < MAX_KILL_TEXT_COUNT - 1; i++)
        floatingKillTexts.at(i).setNext(&floatingKillTexts.at(i + 1));

    // The last one in the list terminates
    floatingKillTexts.at(MAX_KILL_TEXT_COUNT - 1).setNext(nullptr);
}


FloatingKillTexts::FloatingKillText* FloatingKillTexts::FloatingKillText::getNext() const
{
    return next;
}


void FloatingKillTexts::FloatingKillText::setNext(FloatingKillText *next)
{
    this->next = next;
}


void FloatingKillTexts::FloatingKillText::reset()
{
    text.setString({});
    text.setFillColor({255, 255, 255, 0});
    isActive = false;
}


void FloatingKillTexts::FloatingKillText::activate(const int amount, const sf::Vector2f position)
{
    text.setString(std::to_string(amount));
    text.setPosition(position);
    fadeScale = 0.0;
    fadeTime = 0.0;
    isActive = true;
}


bool FloatingKillTexts::FloatingKillText::fadeIn()
{
    if (fadeTime < baseFadeTime) {

        fadeTime += GameRoot::instance().deltaTime;
        fadeScale += GameRoot::instance().deltaTime;

        if (fadeTime < baseFadeTime) {

            // Fade
            auto a = static_cast<std::uint8_t>(255 * (1.0f - fadeTime / baseFadeTime));
            text.setFillColor({255, 255, 255, a});

            // Scale
            auto scale = static_cast<float>(1.10 * (1.0f - fadeScale / baseFadeScale));
            text.setScale({ scale, scale });
            return false;
        }

        // Make sure it is not visible at this point
        text.setFillColor({255, 255, 255, 0});
    }

    // The text is at full fade, reset it
    reset();

    return true;
}


void FloatingKillTexts::add(const int amount, const sf::Vector2f position)
{
    assert(firstAvailable != nullptr);

    FloatingKillText *newFloatingKillText = firstAvailable;
    if (newFloatingKillText->getNext() != nullptr)
    {
        firstAvailable = newFloatingKillText->getNext();
        newFloatingKillText->activate(amount, position);
    }
}


void FloatingKillTexts::update()
{
    for (int i = 0; i < MAX_KILL_TEXT_COUNT; i++)
        if (floatingKillTexts.at(i).isActive)
            if (floatingKillTexts.at(i).fadeIn())
            {
                floatingKillTexts.at(i).setNext(firstAvailable);
                firstAvailable = &floatingKillTexts.at(i);
            }
}


void FloatingKillTexts::draw() const
{
    for (int i = 0; i < MAX_KILL_TEXT_COUNT; i++)
        floatingKillTexts.at(i).draw();
}


void FloatingKillTexts::FloatingKillText::draw() const
{
    if (isActive)
        GameRoot::instance().renderWindow.draw(text);
}

