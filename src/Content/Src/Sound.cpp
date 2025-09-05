#include "../Include/Sound.h"
#include "../../Logger/Logger.h"
#include "SFML/Audio/SoundBuffer.hpp"
#include "SFML/System/Exception.hpp"


Sound::Sound() {

    try {
        // Load the gameplay song
        gameplaySong = sf::Music("Content\\Sound\\Music.mp3");
        gameplaySong.setLoopPoints({sf::milliseconds(0), sf::seconds(gameplaySong.getDuration().asSeconds())});

        // Set the shot volumes
        shot1Sound.setVolume(50.0f);
        shot2Sound.setVolume(50.0f);
        shot3Sound.setVolume(50.0f);
        shot4Sound.setVolume(50.0f);

        // Set the explosion volumes
        explosion1Sound.setVolume(70.0f);
        explosion2Sound.setVolume(70.0f);
        explosion3Sound.setVolume(70.0f);
        explosion4Sound.setVolume(70.0f);
        explosion5Sound.setVolume(70.0f);
        explosion6Sound.setVolume(70.0f);
        explosion7Sound.setVolume(70.0f);
        explosion8Sound.setVolume(70.0f);

        // Set the spawn volumes
        spawn1Sound.setVolume(60.0f);
        spawn2Sound.setVolume(60.0f);
        spawn3Sound.setVolume(60.0f);
        spawn4Sound.setVolume(60.0f);
        spawn5Sound.setVolume(60.0f);
        spawn6Sound.setVolume(60.0f);
        spawn7Sound.setVolume(60.0f);
        spawn8Sound.setVolume(60.0f);

    } catch (sf::Exception &ex) {
        Logger::printError(ex.what());
    }

}


void Sound::togglePlaySounds() {

    playSound = !playSound;

    if (gameplaySong.getStatus() == sf::SoundSource::Status::Playing)
        gameplaySong.stop();
    else
        gameplaySong.play();
}



void Sound::playGamePlaySong()  {

    if (!playSound || gameplaySong.getStatus() == sf::SoundSource::Status::Playing)
        return;

    gameplaySong.play();
}


void Sound::playShotSound() {

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


void Sound::playExplosionSound() {

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


void Sound::playSpawnSound() {

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

