#pragma once
#define INCLUDE_SDL_MIXER
#include "SDL_include.h"
#include <string>

class Sound {
private:
    Mix_Chunk* chunk;
    int channel;
public:
    Sound();
    Sound(std::string file);
    ~Sound();
    void Play(int times = 1);
    void Stop();
    void Open(std::string file);
    bool IsOpen();
};