#include "../Include/Sound.h"
#include "../../GameRoot.h"
#include "SFML/Audio/SoundBuffer.hpp"


Sound::Song::Song(const std::filesystem::path& filename, const bool shouldLoop, const std::int32_t loopStartOffsetMs) : sf::Music(filename)
{
    if (shouldLoop)
    {
        setLooping(true);
        setLoopPoints({sf::milliseconds(loopStartOffsetMs), sf::seconds(getDuration().asSeconds())});
    }
}


void Sound::Song::startPlaying()
{
    if (getStatus() != Status::Playing)
        play();
}


void Sound::Song::stopPlaying()
{
    if (getStatus() != Status::Stopped)
        stop();
}


void Sound::Song::fadeOut(const float fadeTime)
{
    if (isFadingOut)
        return;

    isFadingOut = true;
    fadeOutDuration = fadeTime;
    fadeOutElapsed = fadeOutDuration;
}


void Sound::Song::fadeIn(const float fadeTime)
{
    if (isFadingIn || getStatus() == Status::Playing)
        return;

    isFadingIn = true;
    fadeInDuration = fadeTime;

    setVolume(0);
    play();
}


void Sound::Song::update()
{
    if (isFadingOut)
    {
        fadeOutElapsed -= GameRoot::instance().deltaTime;

        setVolume(fadeOutElapsed / fadeOutDuration * instance().musicMasterVolume);

        if (fadeOutElapsed < 0.f)
        {
            stop();
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


Sound::Sound()
{
    menuBackgroundSong.setLoopPoints({
        sf::milliseconds(500),
        sf::seconds(menuBackgroundSong.getDuration().asSeconds())
    });
    menuBackgroundSong.setLooping(true);
}


void Sound::setMusicMasterVolume(const float volume)
{
    musicMasterVolume = volume;

    menuBackgroundSong.setVolume(musicMasterVolume);
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
}

