#include "../Include/Sound.h"
#include "../../GameRoot.h"
#include "SFML/Audio/SoundBuffer.hpp"


Sound::Sound()
{
    gameplaySong.lowPassVersion = &gameplaySongLPF;
}


Sound::Song::Song(const std::filesystem::path& filename, const bool shouldLoop, const std::int32_t loopStartOffsetMs) : sf::Music(filename)
{
    if (shouldLoop)
    {
        setLooping(true);
        setLoopPoints({sf::milliseconds(loopStartOffsetMs), sf::seconds(getDuration().asSeconds())});
    }
}


void Sound::Song::muffle()
{
    if (lowPassVersion != nullptr)
    {
        lowPassVersion->setVolume(getVolume());
        setVolume(0.f);
        isMuffled = true;
    }
}


void Sound::Song::unmuffle()
{
    if (lowPassVersion != nullptr)
    {
        lowPassVersion->setVolume(0.f);
        setVolume(instance().musicMasterVolume);
        isMuffled = false;
    }
}


void Sound::Song::startPlaying()
{
    if (getStatus() != Status::Playing)
    {
        play();

        if (lowPassVersion != nullptr)
            lowPassVersion->startPlaying();
    }
}


void Sound::Song::stopPlaying()
{
    if (getStatus() != Status::Stopped)
    {
        stop();

        if (lowPassVersion != nullptr)
            lowPassVersion->stopPlaying();
    }
}


void Sound::Song::fadeOut(const float fadeTime)
{
    if (isFadingOut)
        return;

    isFadingOut = true;
    fadeOutDuration = fadeTime;
    fadeOutElapsed = fadeOutDuration;

    if (lowPassVersion != nullptr)
        lowPassVersion->fadeOut(fadeTime);
}


void Sound::Song::fadeIn(const float fadeTime)
{
    if (isFadingIn || getStatus() == Status::Playing)
        return;

    isFadingIn = true;
    fadeInDuration = fadeTime;

    setVolume(0);
    startPlaying();

    if (lowPassVersion != nullptr)
        lowPassVersion->fadeIn(fadeTime);
}


void Sound::Song::update()
{
    if (isFadingOut)
    {
        fadeOutElapsed -= GameRoot::instance().deltaTime;

        setVolume(fadeOutElapsed / fadeOutDuration * instance().musicMasterVolume);

        if (fadeOutElapsed < 0.f)
        {
            stopPlaying();
            setVolume(instance().musicMasterVolume);
            isFadingOut = false;
            fadeOutElapsed = 0.f;
        }
    }

    if (isFadingIn)
    {
        fadeInElapsed += GameRoot::instance().deltaTime;

        setVolume(fadeInElapsed * instance().musicMasterVolume);

        if (fadeInElapsed > fadeInDuration)
        {
            setVolume(instance().musicMasterVolume);
            isFadingIn = false;
            fadeInElapsed = 0.f;
        }
    }
}


void Sound::setMusicMasterVolume(const float volume)
{
    musicMasterVolume = volume;

    menuBackgroundSong.setVolume(musicMasterVolume);

    if (gameplaySong.isMuffled)
        gameplaySongLPF.setVolume(musicMasterVolume);
    else
        gameplaySong.setVolume(musicMasterVolume);
}


void Sound::setSfxMasterVolume(const float volume)
{
    sfxMasterVolume = volume;

    // Menu sounds
    menuUpSound.setVolume(sfxMasterVolume);
    menuDownSound.setVolume(sfxMasterVolume);
    menuLeftSound.setVolume(sfxMasterVolume);
    menuRightSound.setVolume(sfxMasterVolume);
    menuOpenSound.setVolume(sfxMasterVolume);
    menuCloseSound.setVolume(sfxMasterVolume);
}

void Sound::update()
{
    menuBackgroundSong.update();
    gameplaySong.update();
    gameplaySongLPF.update();
}

