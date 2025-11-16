#include "../Include/Sound.h"
#include "../../Core/Include/Logger.h"
#include "SFML/Audio/SoundBuffer.hpp"
#include "SFML/System/Exception.hpp"


Sound::Sound()
{
    try {
        // Load the gameplay song
        gameplaySong = sf::Music("Content\\Sound\\Music.mp3");
        gameplaySong.setLoopPoints({sf::milliseconds(0), sf::seconds(gameplaySong.getDuration().asSeconds())});

    } catch (sf::Exception &ex) {
        Logger::printError(ex.what());
    }
}


void Sound::setMusicMasterVolume(const float volume)
{
    musicMasterVolume = volume;
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

    // Set the shot volumes
    shot1Sound.setVolume(sfxMasterVolume);
    shot2Sound.setVolume(sfxMasterVolume);
    shot3Sound.setVolume(sfxMasterVolume);
    shot4Sound.setVolume(sfxMasterVolume);

    // Set the explosion volumes
    explosion1Sound.setVolume(sfxMasterVolume);
    explosion2Sound.setVolume(sfxMasterVolume);
    explosion3Sound.setVolume(sfxMasterVolume);
    explosion4Sound.setVolume(sfxMasterVolume);
    explosion5Sound.setVolume(sfxMasterVolume);
    explosion6Sound.setVolume(sfxMasterVolume);
    explosion7Sound.setVolume(sfxMasterVolume);
    explosion8Sound.setVolume(sfxMasterVolume);

    // Set the spawn volumes
    spawn1Sound.setVolume(sfxMasterVolume);
    spawn2Sound.setVolume(sfxMasterVolume);
    spawn3Sound.setVolume(sfxMasterVolume);
    spawn4Sound.setVolume(sfxMasterVolume);
    spawn5Sound.setVolume(sfxMasterVolume);
    spawn6Sound.setVolume(sfxMasterVolume);
    spawn7Sound.setVolume(sfxMasterVolume);
    spawn8Sound.setVolume(sfxMasterVolume);
}


void Sound::togglePlaySounds()
{
    playSound = !playSound;

    // if (gameplaySong.getStatus() == sf::SoundSource::Status::Playing)
    //     gameplaySong.stop();
    // else
    //     gameplaySong.play();
}


void Sound::playGamePlaySong()
{
    if (!playSound || gameplaySong.getStatus() == sf::SoundSource::Status::Playing)
        return;

    gameplaySong.play();
}


void Sound::playShotSound()
{
    if (!playSound)
        return;

    const int nextShotIndex = shotIndex(randEngine);

    if (nextShotIndex == 1) {
        shot1Sound.setPitch(shotPitch(randEngine));
        shot1Sound.play();
    } else if (nextShotIndex == 2) {
        shot2Sound.setPitch(shotPitch(randEngine));
        shot2Sound.play();
    } else if (nextShotIndex == 3) {
        shot3Sound.setPitch(shotPitch(randEngine));
        shot3Sound.play();
    } else if (nextShotIndex == 4) {
        shot4Sound.setPitch(shotPitch(randEngine));
        shot4Sound.play();
    }
}


void Sound::playExplosionSound()
{
    if (!playSound)
        return;

    const int nextExplosionIndex = explosionIndex(randEngine);

    if (nextExplosionIndex == 1) {
        explosion1Sound.setPitch(explosionPitch(randEngine));
        explosion1Sound.play();
    } else if (nextExplosionIndex == 2) {
        explosion2Sound.setPitch(explosionPitch(randEngine));
        explosion2Sound.play();
    } else if (nextExplosionIndex == 3) {
        explosion3Sound.setPitch(explosionPitch(randEngine));
        explosion3Sound.play();
    } else if (nextExplosionIndex == 4) {
        explosion4Sound.setPitch(explosionPitch(randEngine));
        explosion4Sound.play();
    } else if (nextExplosionIndex == 5) {
        explosion5Sound.setPitch(explosionPitch(randEngine));
        explosion5Sound.play();
    } else if (nextExplosionIndex == 6) {
        explosion6Sound.setPitch(explosionPitch(randEngine));
        explosion6Sound.play();
    } else if (nextExplosionIndex == 7) {
        explosion7Sound.setPitch(explosionPitch(randEngine));
        explosion7Sound.play();
    } else if (nextExplosionIndex == 8) {
        explosion8Sound.setPitch(explosionPitch(randEngine));
        explosion8Sound.play();
    }
}


void Sound::playSpawnSound()
{
    if (!playSound)
        return;

    const int nextSpawnIndex = spawnIndex(randEngine);

    if (nextSpawnIndex == 1) {
        spawn1Sound.setPitch(spawnPitch(randEngine));
        spawn1Sound.play();
    } else if (nextSpawnIndex == 2) {
        spawn2Sound.setPitch(spawnPitch(randEngine));
        spawn2Sound.play();
    } else if (nextSpawnIndex == 3) {
        spawn3Sound.setPitch(spawnPitch(randEngine));
        spawn3Sound.play();
    } else if (nextSpawnIndex == 4) {
        spawn4Sound.setPitch(spawnPitch(randEngine));
        spawn4Sound.play();
    } else if (nextSpawnIndex == 5) {
        spawn5Sound.setPitch(spawnPitch(randEngine));
        spawn5Sound.play();
    } else if (nextSpawnIndex == 6) {
        spawn6Sound.setPitch(spawnPitch(randEngine));
        spawn6Sound.play();
    } else if (nextSpawnIndex == 7) {
        spawn7Sound.setPitch(spawnPitch(randEngine));
        spawn7Sound.play();
    } else if (nextSpawnIndex == 8) {
        spawn8Sound.setPitch(spawnPitch(randEngine));
        spawn8Sound.play();
    }
}

