#include <cmath>
#include "../Include/Buffs.h"
#include "../../GameRoot.h"
#include "../../Content/Include/GaussianBlur.h"
#include "../../System/Include/Extensions.h"


Buffs::Buffs()
{
    // Buff2 alignment
    buff2.buffFrame.setOrigin({
        buff2.buffFrame.getTexture().getSize().x / 2.f,
        buff2.buffFrame.getTexture().getSize().y / 2.f
    });
    buff2.buffFrame.setPosition({
        GameRoot::instance().windowSizeF.x / 2.f - 40.f,
        GameRoot::instance().windowSizeF.y - 80.f
    });
    buff2.buffIcon.setTexture(Art::instance().invincibleBuff);
    buff2.buffIcon.setScale({0.75f, 0.75f});
    buff2.buffIcon.setRotation(sf::radians(-PI / 2));
    buff2.buffIcon.setOrigin({
        buff2.buffIcon.getTexture().getSize().x / 2.f,
        buff2.buffIcon.getTexture().getSize().y / 2.f
    });
    buff2.buffIcon.setPosition(buff2.buffFrame.getPosition());


    // Buff1 alignment
    buff1.buffFrame.setOrigin({
        buff1.buffFrame.getTexture().getSize().x / 2.f,
        buff1.buffFrame.getTexture().getSize().y / 2.f
    });
    buff1.buffFrame.setPosition({
        buff2.buffFrame.getPosition().x - 80.f,
        GameRoot::instance().windowSizeF.y - 80.f
    });
    buff1.buffIcon.setTexture(Art::instance().bulletsAllDirectionsBuff);
    buff1.buffIcon.setOrigin({
        buff1.buffIcon.getTexture().getSize().x / 2.f,
        buff1.buffIcon.getTexture().getSize().y / 2.f
    });
    buff1.buffIcon.setPosition(buff1.buffFrame.getPosition());

    // Buff3 alignment
    buff3.buffFrame.setOrigin({
        buff3.buffFrame.getTexture().getSize().x / 2.f,
        buff3.buffFrame.getTexture().getSize().y / 2.f
    });
    buff3.buffFrame.setPosition({
        GameRoot::instance().windowSizeF.x / 2.f + 40.f,
        GameRoot::instance().windowSizeF.y - 80.f
    });
    buff3.buffIcon.setTexture(Art::instance().shotGunBuff);
    buff3.buffIcon.setOrigin({
        buff3.buffIcon.getTexture().getSize().x / 2.f,
        buff3.buffIcon.getTexture().getSize().y / 2.f
    });
    buff3.buffIcon.setPosition(buff3.buffFrame.getPosition());

    // Buff4 alignment
    buff4.buffFrame.setOrigin({
        buff4.buffFrame.getTexture().getSize().x / 2.f,
        buff4.buffFrame.getTexture().getSize().y / 2.f
    });
    buff4.buffFrame.setPosition({
        buff3.buffFrame.getPosition().x + 80.f,
        GameRoot::instance().windowSizeF.y - 80.f
    });
    buff4.buffIcon.setTexture(Art::instance().boostersBuff);
    buff4.buffIcon.setOrigin({
        buff4.buffIcon.getTexture().getSize().x / 2.f,
        buff4.buffIcon.getTexture().getSize().y / 2.f
    });
    buff4.buffIcon.setPosition(buff4.buffFrame.getPosition());

}


void Buffs::draw()
{
    std::uint8_t alpha = static_cast<std::uint8_t>(255 * GameRoot::instance().frameUIOpacity);

    buff1.buffFrame.setColor({255, 255, 255, alpha});
    GaussianBlur::instance().drawToBase(buff1.buffFrame);
    GaussianBlur::instance().drawToBase(buff1.buffIcon);

    buff2.buffFrame.setColor({255, 255, 255, alpha});
    GaussianBlur::instance().drawToBase(buff2.buffFrame);
    GaussianBlur::instance().drawToBase(buff2.buffIcon);

    buff3.buffFrame.setColor({255, 255, 255, alpha});
    GaussianBlur::instance().drawToBase(buff3.buffFrame);
    GaussianBlur::instance().drawToBase(buff3.buffIcon);

    buff4.buffFrame.setColor({255, 255, 255, alpha});
    GaussianBlur::instance().drawToBase(buff4.buffFrame);
    GaussianBlur::instance().drawToBase(buff4.buffIcon);
}
