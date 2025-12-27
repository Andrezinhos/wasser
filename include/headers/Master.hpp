#pragma once
#include <SDL.h>          
#include <SDL_mixer.h>  
#include <filesystem>  
#include <string>
#include "Playlist.hpp"
#include "History.hpp"
#include "Track.hpp"
#include "states.hpp"

class AudioPlayer{
public:

    AudioPlayer();
    ~AudioPlayer();

    bool init();
    void load(const std::string& file);
    void play();
    void multiPlay(const std::string& path, int channel);
    void pauseChannel(int channel);
    void resumeChannel(int channel);
    void stopChannel(int channel);
    void setChannelVol(int channel, int vol);
    void pause();
    void previous();
    void resume();
    void stop();
    void update();
    void songCount() const;
    void delSong(const std::string& name);

    bool isSongPresent(const std::string& song) const;
    void onSongEnded();
    void addToQueue(const std::string& song);
    void next();
    void fadeIn(int ms);
    void fadeOut(int ms);
    void volSet(int value);

    Playlist playlist;
    History history;
    Track queue;

private:
    bool isPlaying;
    bool isPaused;
    Mix_Music* music;
};
    