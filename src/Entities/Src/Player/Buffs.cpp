#include <cmath>
#include "../../Include/Player/Buffs.h"
#include "../../../GameRoot.h"
#include "../../../Content/Include/GaussianBlur.h"
#include "../../../Core/Include/Extensions.h"
#include "../../Include/Player/PlayerShip.h"
#include "SFML/Graphics/Text.hpp"


Buffs::Buffs()
{
    // The positing of the buffs is static so set the frames and positions
    // Buff2 alignment
    buff2.frame.setOrigin({
        buff2.frame.getTexture().getSize().x / 2.f,
        buff2.frame.getTexture().getSize().y / 2.f
    });
    buff2.frame.setPosition({
        GameRoot::instance().windowSizeF.x / 2.f - 40.f,
        GameRoot::instance().windowSizeF.y - 80.f
    });
    buff2.icon.setPosition(buff2.frame.getPosition());
    buff2.timerText.setPosition(buff2.frame.getPosition());
    buff2.timerText.setStyle(sf::Text::Bold);

    // Buff1 alignment
    buff1.frame.setOrigin({
        buff1.frame.getTexture().getSize().x / 2.f,
        buff1.frame.getTexture().getSize().y / 2.f
    });
    buff1.frame.setPosition({
        buff2.frame.getPosition().x - 80.f,
        GameRoot::instance().windowSizeF.y - 80.f
    });
    buff1.icon.setPosition(buff1.frame.getPosition());
    buff1.timerText.setPosition(buff1.frame.getPosition());
    buff1.timerText.setStyle(sf::Text::Bold);

    // Buff3 alignment
    buff3.frame.setOrigin({
        buff3.frame.getTexture().getSize().x / 2.f,
        buff3.frame.getTexture().getSize().y / 2.f
    });
    buff3.frame.setPosition({
        GameRoot::instance().windowSizeF.x / 2.f + 40.f,
        GameRoot::instance().windowSizeF.y - 80.f
    });
    buff3.icon.setPosition(buff3.frame.getPosition());
    buff3.timerText.setPosition(buff3.frame.getPosition());
    buff3.timerText.setStyle(sf::Text::Bold);

    // Buff4 alignment
    buff4.frame.setOrigin({
        buff4.frame.getTexture().getSize().x / 2.f,
        buff4.frame.getTexture().getSize().y / 2.f
    });
    buff4.frame.setPosition({
        buff3.frame.getPosition().x + 80.f,
        GameRoot::instance().windowSizeF.y - 80.f
    });
    buff4.icon.setPosition(buff4.frame.getPosition());
    buff4.timerText.setPosition(buff4.frame.getPosition());
    buff4.timerText.setStyle(sf::Text::Bold);
}


void Buffs::resetBuffDrops()
{
    buffDrop1.reset();
    buffDrop2.reset();
    buffDrop3.reset();
}


void Buffs::resetBuffs()
{
    buff1.reset();
    buff2.reset();
    buff3.reset();
    buff4.reset();
}


void Buffs::BuffDrop::reset()
{
    isActive = false;
    isPickedUp = false;
    icon.setTexture(Art::instance().buffIconPlaceHolder);
    icon.setScale({1.f, 1.f});
    type = BuffType::None;
}


void Buffs::Buff::reset()
{
    isActive = false;
    duration = 0.f;
    icon.setTexture(Art::instance().buffIconPlaceHolder);
    icon.setColor(sf::Color::White);
    icon.setRotation(sf::Angle::Zero);
    icon.setScale({1.f, 1.f});
    timerText.setString({"0"});
    type = BuffType::None;
}


sf::Vector2f Buffs::BuffDrop::getPosition() const
{
    return icon.getPosition();
}


bool Buffs::BuffDrop::availableForPickUp() const
{
    return isActive && !isPickedUp;
}


bool Buffs::canPickUpBuff()
{
    return nextAvailableBuff() != nullptr;
}


void Buffs::checkBuffDrop(const sf::Vector2f& position)
{
    // Only allowing a chance for a buff drop once a second
    if (allowBuffDropChance)
    {
        allowBuffDropChance = false;

        // Check drop chance
        BuffDrop* next = nextAvailableBuffDrop();
        if (buffDropDistribution(randEngine) <= DROP_RATE && next != nullptr)
        {
            switch (buffTypeDistribution(randEngine))
            {
            case 1:
                next->activate(BuffType::BulletsAllDirections, position);
                break;
            case 2:
                next->activate(BuffType::Shotgun, position);
                break;
            case 3:
                next->activate(BuffType::Invincible, position);
                break;
            case 4:
                next->activate(BuffType::Boosters, position);
                break;
            }
        }
    }
}


Buffs::BuffDrop* Buffs::nextAvailableBuffDrop()
{
    if (!buffDrop1.isActive)
        return &buffDrop1;

    if (!buffDrop2.isActive)
        return &buffDrop2;

    if (!buffDrop3.isActive)
        return &buffDrop3;

    return nullptr;
}


Buffs::Buff* Buffs::nextAvailableBuff()
{
    if (!buff1.isActive)
        return &buff1;

    if (!buff2.isActive)
        return &buff2;

    if (!buff3.isActive)
        return &buff3;

    if (!buff4.isActive)
        return &buff4;

    return nullptr;
}


void Buffs::BuffDrop::activate(const BuffType buffType, const sf::Vector2f position)
{
    type = buffType;
    isActive = true;
    icon.setPosition(position);

    switch (type)
    {
    case BuffType::BulletsAllDirections:
        icon.setTexture(Art::instance().bulletsAllDirectionsBuff);
        break;
    case BuffType::Shotgun:
        icon.setTexture(Art::instance().shotGunBuff);
        break;
    case BuffType::Invincible:
        icon.setTexture(Art::instance().invincibleBuff);
        break;
    case BuffType::Boosters:
        icon.setTexture(Art::instance().boostersBuff);
        break;
    case BuffType::None:
    default:
        icon.setTexture(Art::instance().buffIconPlaceHolder);
    }

    // Set the origin
    icon.setOrigin({
        icon.getTexture().getSize().x / 2.f,
        icon.getTexture().getSize().y / 2.f
    });
}


void Buffs::Buff::activate(const BuffType buffType)
{
    type = buffType;
    isActive = true;

    switch (type)
    {
    case BuffType::BulletsAllDirections:
        icon.setTexture(Art::instance().bulletsAllDirectionsBuff);
        break;
    case BuffType::Shotgun:
        icon.setTexture(Art::instance().shotGunBuff);
        break;
    case BuffType::Invincible:
        icon.setTexture(Art::instance().invincibleBuff);
        icon.setScale({0.75f, 0.75f});
        icon.setRotation(sf::radians(-PI / 2));
        break;
    case BuffType::Boosters:
        icon.setTexture(Art::instance().boostersBuff);
        break;
    case BuffType::None:
    default:
        icon.setTexture(Art::instance().buffIconPlaceHolder);
    }

    // Set the origin
    icon.setOrigin({
        icon.getTexture().getSize().x / 2.f,
        icon.getTexture().getSize().y / 2.f
    });
}


bool Buffs::Buff::isBeingUsed() const
{
    return duration > 0.f;
}


void Buffs::Buff::setUsing()
{
    // Set the icon opacity low to draw timer text over it
    icon.setColor({255, 255, 255, 48});
    duration = maxBuffDuration;
}


void Buffs::Buff::use()
{
    if (isActive && !isBeingUsed())
    {
        switch (type)
        {
        case BuffType::BulletsAllDirections:
            // Make sure neither all directions nor shotgun is currently being used
            if (!PlayerShip::instance().isUsingBulletsAllDirections && !PlayerShip::instance().isUsingShotgunBullets)
            {
                PlayerShip::instance().setUsingBulletsAllDirections(true);
                setUsing();
            }
            break;
        case BuffType::Shotgun:
            // Make sure neither all directions nor shotgun is currently being used
            if (!PlayerShip::instance().isUsingShotgunBullets && !PlayerShip::instance().isUsingBulletsAllDirections)
            {
                PlayerShip::instance().setUsingShotgunBullets(true);
                setUsing();
            }
            break;
        case BuffType::Invincible:
            if (!PlayerShip::instance().isInvincible)
            {
                PlayerShip::instance().setInvincibility(true);
                setUsing();
            }
            break;
        case BuffType::Boosters:
            if (!PlayerShip::instance().isUsingBoosters)
            {
                PlayerShip::instance().setUsingBoosters(true);
                setUsing();
            }
            break;
        }
    }
}


void Buffs::useBuff(const int buffNumber)
{
    if (buffNumber == 1)
    {
        buff1.use();
        return;
    }

    if (buffNumber == 2)
    {
        buff2.use();
        return;
    }

    if (buffNumber == 3)
    {
        buff3.use();
        return;
    }

    if (buffNumber == 4)
        buff4.use();
}


void Buffs::update()
{
    // Time until next buff
    timeUntilPotentialBuffDrop -= GameRoot::instance().deltaTime;
    if (timeUntilPotentialBuffDrop <= 0.f)
    {
        timeUntilPotentialBuffDrop = POTENTIAL_BUFF_DROP_INTERVAL;
        allowBuffDropChance = true;
    }

    // Update active buff drops
    buffDrop1.update();
    buffDrop2.update();
    buffDrop3.update();

    // Update buffs
    buff1.update();
    buff2.update();
    buff3.update();
    buff4.update();
}


void Buffs::BuffDrop::update()
{
    if (isActive)
    {
        if (isPickedUp)
        {
            Buff* next = instance().nextAvailableBuff();
            if (next != nullptr)
                next->activate(type);

            reset();
            return;
        }

        // Pulse the icon
        float scale = 1.1f + 0.1f * std::sin(4.f * GameRoot::instance().elapsedGameTime);
        icon.setScale({scale, scale});
    }
}


void Buffs::Buff::update()
{
    if (isBeingUsed())
    {
        duration -= GameRoot::instance().deltaTime;

        if (duration < 1.f)
        {
            switch (type)
            {
            case BuffType::BulletsAllDirections:
                PlayerShip::instance().setUsingBulletsAllDirections(false);
                break;
            case BuffType::Shotgun:
                PlayerShip::instance().setUsingShotgunBullets(false);
                break;
            case BuffType::Invincible:
                PlayerShip::instance().setInvincibility(false);
                break;
            case BuffType::Boosters:
                PlayerShip::instance().setUsingBoosters(false);
                break;
            }

            reset();
        }

        // Update the timer text
        timerText.setString(std::to_string(static_cast<int>(duration)));
        timerText.setOrigin(timerText.getLocalBounds().getCenter());
    }
}


void Buffs::draw()
{
    std::uint8_t alpha = static_cast<std::uint8_t>(255 * GameRoot::instance().frameUIOpacity);

    buff1.frame.setColor({255, 255, 255, alpha});
    GaussianBlur::instance().drawToBase(buff1.frame);
    GaussianBlur::instance().drawToBase(buff1.icon);

    buff2.frame.setColor({255, 255, 255, alpha});
    GaussianBlur::instance().drawToBase(buff2.frame);
    GaussianBlur::instance().drawToBase(buff2.icon);

    buff3.frame.setColor({255, 255, 255, alpha});
    GaussianBlur::instance().drawToBase(buff3.frame);
    GaussianBlur::instance().drawToBase(buff3.icon);

    buff4.frame.setColor({255, 255, 255, alpha});
    GaussianBlur::instance().drawToBase(buff4.frame);
    GaussianBlur::instance().drawToBase(buff4.icon);

    // Buff drops
    buffDrop1.draw();
    buffDrop2.draw();
    buffDrop3.draw();
}


void Buffs::drawText() const
{
    if (buff1.isBeingUsed())
        GameRoot::instance().renderWindow.draw(buff1.timerText);

    if (buff2.isBeingUsed())
        GameRoot::instance().renderWindow.draw(buff2.timerText);

    if (buff3.isBeingUsed())
        GameRoot::instance().renderWindow.draw(buff3.timerText);

    if (buff4.isBeingUsed())
        GameRoot::instance().renderWindow.draw(buff4.timerText);
}



void Buffs::BuffDrop::draw() const
{
    if (isActive)
        GaussianBlur::instance().drawToBase(icon);
}
