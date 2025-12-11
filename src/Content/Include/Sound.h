#ifndef SOUND_H
#define SOUND_H


#include "SFML/Audio/Music.hpp"
#include "SFML/Audio/Sound.hpp"
#include "SFML/Audio/SoundBuffer.hpp"


class Sound {
private:
    bool playSound = false;
    float musicMasterVolume = 100.f;
    float sfxMasterVolume = 100.f;

    sf::SoundBuffer menuDownSoundBuffer = sf::SoundBuffer("Content\\Sound\\MenuSounds\\MenuSoundDown.ogg");
    sf::SoundBuffer menuUpSoundBuffer = sf::SoundBuffer("Content\\Sound\\MenuSounds\\MenuSoundUp.ogg");
    sf::SoundBuffer menuLeftSoundBuffer = sf::SoundBuffer("Content\\Sound\\MenuSounds\\MenuSoundLeft.ogg");
    sf::SoundBuffer menuRightSoundBuffer = sf::SoundBuffer("Content\\Sound\\MenuSounds\\MenuSoundRight.ogg");
    sf::SoundBuffer menuOpenSoundBuffer = sf::SoundBuffer("Content\\Sound\\MenuSounds\\MenuOpen.ogg");
    sf::SoundBuffer menuCloseSoundBuffer = sf::SoundBuffer("Content\\Sound\\MenuSounds\\MenuClose.ogg");

    struct Song final : sf::Music
    {
        explicit Song(const std::filesystem::path& filename, bool shouldLoop, std::int32_t loopStartOffsetMs);

        float fadeOutDuration = 1.f;
        float fadeOutElapsed = 0.f;
        bool isFadingOut = false;

        float fadeInDuration = 1.f;
        float fadeInElapsed = 0.f;
        bool isFadingIn = false;

        void startPlaying();
        void stopPlaying();
        void fadeOut(float fadeTime);
        void fadeIn(float fadeTime);
        void update();
    };

public:
    Sound();

    static Sound &instance() {
        static auto *instance = new Sound;
        return *instance;
    }

    Song menuBackgroundSong {"Content\\Sound\\MenuSounds\\MenuBackground.ogg", true, 600};
    sf::Sound menuUpSound = sf::Sound(menuUpSoundBuffer);
    sf::Sound menuDownSound = sf::Sound(menuDownSoundBuffer);
    sf::Sound menuLeftSound = sf::Sound(menuLeftSoundBuffer);
    sf::Sound menuRightSound = sf::Sound(menuRightSoundBuffer);
    sf::Sound menuOpenSound = sf::Sound(menuOpenSoundBuffer);
    sf::Sound menuCloseSound = sf::Sound(menuCloseSoundBuffer);

    void setMusicMasterVolume(float volume);
    void setSfxMasterVolume(float volume);
    void update();
};



#endif //SOUND_H
