#include <iostream>
#include <filesystem>
#include <algorithm> 
#include "Master.hpp"

namespace fs = std::filesystem;

AudioPlayer::AudioPlayer() : isPlaying(false), isPaused(true), music(nullptr) {}

AudioPlayer::~AudioPlayer(){
    if(music){
        Mix_FreeMusic(music);   
        music = nullptr;
    }
    Mix_CloseAudio();
    SDL_Quit();
}

AudioPlayer* g_player = nullptr;
void songEndCall(){
    if(g_player){
        g_player->onSongEnded();
    }
}

bool AudioPlayer::init(){
    isPlaying = false;
    isPaused = true;

    if(SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cout << "SDL couldn't initialize!: " << SDL_GetError() << "\n";
        return false;
    }

    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout << "SDL_mixer couldn't initialize!: " << Mix_GetError() << "\n";
        return false;
    }

    g_player = this;
    Mix_HookMusicFinished(songEndCall);
    return true;
}

void AudioPlayer::onSongEnded(){
    isPlaying = false;
    isPaused = true;
    next();
}

bool AudioPlayer::isSongPresent(const std::string& song) const {
    std::string path = "C:/Wasser/bin/songs/" + song;
    return fs::exists(path);
}

void AudioPlayer::addToQueue(const std::string& song) {
    fs::path fullPath = "C:/Wasser/bin/songs/" + song;
    if(!isSongPresent(song)){
        std::cout << "Song not found consider checking the spellings" << "\n";
        return;
    }   
    if (!queue.enqueue(fullPath.string())) { 
        return; 
    }
    
    playlist.addMediaAtLast(fullPath);

    if (state.debugMode){
        std::cout << "DEBUG: head = " << playlist.head << ", curr = " << playlist.curr << "\n";
    }

    if (!playlist.curr){
        playlist.curr = playlist.head;
    }    
}

void AudioPlayer::next(){
    if(queue.isEmpty()){
        std::cerr << "Playlist is empty\n" << "\n";
        return;
    }

    if(!isPaused) pause();

    std::string temp = playlist.curr->songPath.string();
    playlist.nextTrack();
    history.push(temp);

    if(!queue.isEmpty())queue.dequeue();

    if(history.size() == playlist.getSize()) {
        std::cout << "\nplaylist ended" << "\n";
        std::cout << "replay? Y/N: ";
        char a;
        std::cin >> a;
        if (a=='y'||a=='Y') {
            state.clearScreen();
            queue.clear();
            history.clear();
            playlist.curr = playlist.head;
            Node* ptr = playlist.head;
            do{
                queue.enqueue(ptr->songPath.string());
                ptr = ptr->next;
            } while(ptr != playlist.head);
        } else {
            std::cout << "Stop playing\n" << "\n";
            isPlaying = false;
            return;
        }
    }
    play();
}

void AudioPlayer::previous(){
    if(history.isEmpty()){
        std::cout << "No previous song in history\n" << "\n";
        return;
    }
    pause();
    std::string temp = playlist.curr->songPath.string();
    queue.front();
    playlist.previousTrack();
    history.pop();

    play();
}

void AudioPlayer::play(){
    if (state.debugMode){
        std::cout << "DEBUG: curr pointer = " << playlist.curr << "\n"; 
    }
     if (playlist.curr && state.debugMode) { 
        std::cout << "DEBUG: songPath = " << playlist.curr->songPath << "\n"; 
    }
    if(playlist.curr == nullptr) {
        std::cout << "No song to play\n" << "\n";
        return;
    }

    auto filepath = playlist.curr->songPath;
    std::string temp = filepath.filename().string();

    if(isPlaying && !isPaused){
        std::cout << "Already playing: " << temp << "\n";
        return;
    }

    if(!fs::exists(filepath)){
        std::cout << "File not found: " << Mix_GetError() << "\n";
        return;
    }

    music = Mix_LoadMUS(filepath.string().c_str());
    if (!music){
        std::cout << "Fail to load: " << filepath << "\n";
        return;
    }

    if (state.OnLoop == true) Mix_PlayMusic(music, -1);
    else Mix_PlayMusic(music, 1);

    std::cout << "Playing: " << temp << "\n";

    isPlaying = true;
    isPaused = false;
    
    if(!queue.isEmpty() && fs::path(queue.front().filename().string()) == filepath.string()) queue.dequeue();
}

void AudioPlayer::multiPlay(const std::string& path, int channel){
    Mix_Chunk* sound = Mix_LoadWAV(path.c_str());
    if (!sound) {
        std::cout << "Error to load sound: " << Mix_GetError() << "\n";
        return;
    } 
    Mix_PlayChannel(channel, sound, 0);
    std::cout << "playing: " << path << " in the channel: " << channel << "\n";
}

void AudioPlayer::resumeChannel(int channel){
    Mix_Resume(channel);
    std::cout << "Resumed: " << channel << "\n";
}

void AudioPlayer::pauseChannel(int channel){ 
    Mix_Pause(channel);
    std::cout << "Paused: " << channel << "\n";
}

void AudioPlayer::stopChannel(int channel){ 
    Mix_HaltChannel(channel);
    std::cout << "Stoped: " << channel << "\n";
}

void AudioPlayer::setChannelVol(int channel, int vol){ 
    vol = std::clamp(vol, 0, MIX_MAX_VOLUME);
    Mix_Volume(channel, 0);
    std::cout << "Channel: " << channel << " Vol Set to:" << vol << "\n";
}

void AudioPlayer::pause(){
    if (!playlist.curr) {
        std::cout << "No song Loaded\n" << "\n";
        return;
    }

    std::string temp = fs::path(playlist.curr->songPath).filename().string();

    if(isPlaying && !isPaused) {
        Mix_PauseMusic();
        std::cout << "Paused: " << temp << "\n";
        isPaused = true;
    } else if(!isPlaying) {
        std::cout << "No Song is Playing" << "\n";
    } else {
        std::cout << "Already paused\n";
    }
}

void AudioPlayer::resume(){
    if (playlist.curr && isPaused) {
        Mix_ResumeMusic();
        std::string temp = fs::path(playlist.curr->songPath).filename().string();
        std::cout << "Resumed: " << temp << "\n";
        isPaused = false;
    } else if(isPlaying) {
        std::cout << "Current Song is Playing" << "\n";
    } else {
        std::cout << "Already playing\n";
    }
}

void AudioPlayer::stop(){
    if (!isPlaying){
        std::cout << "No song is playing\n" << "\n";
        return;
    }

    Mix_HaltMusic();
    auto filepath = playlist.curr->songPath;
    std::string temp = filepath.filename().string();
    std::cout << "Stoped: " << temp << "\n";
    
    isPlaying = false;
    isPaused = true;
}

void AudioPlayer::update(){
    if(isPlaying && !Mix_PlayingMusic()) {
        isPaused = true;
        isPlaying = false;
        next();
    }
}

void AudioPlayer::songCount() const {
    std::cout << "The total songs in the playlist are " << playlist.numberOfSongs();
}

void AudioPlayer::delSong(const std::string& name){
    if(!isSongPresent(name)){
        std::cout << "Song not found consider checking the spellings" << "\n";
        return;
    }

    if(!isPaused) pause();
    int result = playlist.deleteMedia("../songs/"+name);
    if(result == 0) {
        std::cout << "Empty Playlist" << "\n";
        return;
    }
    else if(result == 1) {
        std::cout << "Song not in the Playlist" << "\n";
        return;
    }
    else if(result == 2){
        history.clear();
        queue.clear();
        std::cout << "Playlist has been emptied";
        return;
    }
    else if(result == 3){
        queue.dequeue();
        std::cout << "The current song was deleted" << "\n";
    }
    else if(result == 4){
        history.delFromStack("../songs/" + name);
        queue.deleteFromQueue("../songs/" + name);
        std::cout << "The song has been deleted" << "\n";
    }
    if(!isPaused) play();
}
void AudioPlayer::fadeIn(int ms){
    if (!playlist.curr){
        std::cout << "No song to play\n" << "\n";
        return;
    }

    auto filepath = playlist.curr->songPath;
    music = Mix_LoadMUS(filepath.string().c_str());
    if (!music){
        std::cout << "Fail to load: " << filepath << "\n";
        return;
    }

    if (Mix_FadeInMusic(music, 1, ms) == -1){
        std::cout << "Fade in failed: " << Mix_GetError() << "\n";
        return;
    }

    std::cout << "!!! FADE IN: " << filepath.filename().string() << " (" << ms << " ms)\n";

    isPlaying = true;
    isPaused = false;
}

void AudioPlayer::fadeOut(int ms){
    if (!isPlaying){
        std::cout << "No song is playing\n" << "\n";
        return;
    }

    if (Mix_FadeOutMusic(ms) == 0){
        std::cout << "!!! FADE IN: " << " (" << ms << " ms)\n";
    } else {
        std::cout << "Fade out failed: " << Mix_GetError() << "\n";
    }
    
    isPlaying = false;
    isPaused = true;
}

void AudioPlayer::volSet(int value) {
    int vol = std::clamp(value, 0, MIX_MAX_VOLUME);
    Mix_VolumeMusic(vol);
    std::cout << "VOLUME SET TO: " << vol << "\n";
}
