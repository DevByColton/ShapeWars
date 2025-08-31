#ifndef SOUND_H
#define SOUND_H


#include "SFML/Audio/Music.hpp"
#include "SFML/Audio/Sound.hpp"
#include <array>
#include <random>
#include "SFML/Audio/SoundBuffer.hpp"


struct Sound {
    Sound();
    ~Sound() = default;

    static Sound &instance() {
        static auto *instance = new Sound;
        return *instance;
    }

    sf::Music gameplaySong;

    // Shot sounds
    sf::SoundBuffer shot1Buff = sf::SoundBuffer("Content\\Sound\\shoot-01.wav");
    sf::Sound shot1Sound = sf::Sound(shot1Buff);
    sf::SoundBuffer shot2Buff = sf::SoundBuffer("Content\\Sound\\shoot-02.wav");
    sf::Sound shot2Sound = sf::Sound(shot2Buff);
    sf::SoundBuffer shot3Buff = sf::SoundBuffer("Content\\Sound\\shoot-03.wav");
    sf::Sound shot3Sound = sf::Sound(shot3Buff);
    sf::SoundBuffer shot4Buff = sf::SoundBuffer("Content\\Sound\\shoot-04.wav");
    sf::Sound shot4Sound = sf::Sound(shot4Buff);

    // Explosion sounds
    sf::SoundBuffer explosion1Buff = sf::SoundBuffer("Content\\Sound\\explosion-01.wav");
    sf::Sound explosion1Sound = sf::Sound(explosion1Buff);
    sf::SoundBuffer explosion2Buff = sf::SoundBuffer("Content\\Sound\\explosion-02.wav");
    sf::Sound explosion2Sound = sf::Sound(explosion2Buff);
    sf::SoundBuffer explosion3Buff = sf::SoundBuffer("Content\\Sound\\explosion-03.wav");
    sf::Sound explosion3Sound = sf::Sound(explosion3Buff);
    sf::SoundBuffer explosion4Buff = sf::SoundBuffer("Content\\Sound\\explosion-04.wav");
    sf::Sound explosion4Sound = sf::Sound(explosion4Buff);
    sf::SoundBuffer explosion5Buff = sf::SoundBuffer("Content\\Sound\\explosion-05.wav");
    sf::Sound explosion5Sound = sf::Sound(explosion5Buff);
    sf::SoundBuffer explosion6Buff = sf::SoundBuffer("Content\\Sound\\explosion-06.wav");
    sf::Sound explosion6Sound = sf::Sound(explosion6Buff);
    sf::SoundBuffer explosion7Buff = sf::SoundBuffer("Content\\Sound\\explosion-07.wav");
    sf::Sound explosion7Sound = sf::Sound(explosion7Buff);
    sf::SoundBuffer explosion8Buff = sf::SoundBuffer("Content\\Sound\\explosion-08.wav");
    sf::Sound explosion8Sound = sf::Sound(explosion8Buff);

    // Spawn sounds
    sf::SoundBuffer spawn1Buff = sf::SoundBuffer("Content\\Sound\\spawn-01.wav");
    sf::Sound spawn1Sound = sf::Sound(spawn1Buff);
    sf::SoundBuffer spawn2Buff = sf::SoundBuffer("Content\\Sound\\spawn-02.wav");
    sf::Sound spawn2Sound = sf::Sound(spawn2Buff);
    sf::SoundBuffer spawn3Buff = sf::SoundBuffer("Content\\Sound\\spawn-03.wav");
    sf::Sound spawn3Sound = sf::Sound(spawn3Buff);
    sf::SoundBuffer spawn4Buff = sf::SoundBuffer("Content\\Sound\\spawn-04.wav");
    sf::Sound spawn4Sound = sf::Sound(spawn4Buff);
    sf::SoundBuffer spawn5Buff = sf::SoundBuffer("Content\\Sound\\spawn-05.wav");
    sf::Sound spawn5Sound = sf::Sound(spawn5Buff);
    sf::SoundBuffer spawn6Buff = sf::SoundBuffer("Content\\Sound\\spawn-06.wav");
    sf::Sound spawn6Sound = sf::Sound(spawn6Buff);
    sf::SoundBuffer spawn7Buff = sf::SoundBuffer("Content\\Sound\\spawn-07.wav");
    sf::Sound spawn7Sound = sf::Sound(spawn7Buff);
    sf::SoundBuffer spawn8Buff = sf::SoundBuffer("Content\\Sound\\spawn-08.wav");
    sf::Sound spawn8Sound = sf::Sound(spawn8Buff);

    std::default_random_engine randEngine {std::random_device{}()};
    std::uniform_int_distribution<int> shotIndex {1, 4 };
    std::uniform_real_distribution<float> shotPitch {0.6f, 1.0f };
    std::uniform_int_distribution<int> explosionIndex {1, 8 };
    std::uniform_real_distribution<float> explosionPitch {0.6f, 1.0f };
    std::uniform_int_distribution<int> spawnIndex {1, 8 };
    std::uniform_real_distribution<float> spawnPitch {0.6f, 1.0f };

    void playGamePlaySong();
    void playShotSound();
    void playExplosionSound();
    void playSpawnSound();
};



#endif //SOUND_H
