#define INCLUDE_SDL_MIXER
#include "SDL_include.h"

#include "Sound.h"
#include "Resources.h"
#include <iostream>

Sound::Sound() : chunk(nullptr), channel(-1) {}

Sound::Sound(const std::string& file) : Sound() {
  Open(file);
}

Sound::~Sound() {
  Stop(); 
}

void Sound::Play(int times) {
  if (!chunk) return;
  if (times < 1) times = 1;
  int loops = times - 1; 
  if (channel != -1 && Mix_Playing(channel)) {
    return; 
  }

  channel = Mix_PlayChannel(-1, chunk, loops);
  if (channel == -1) {
    std::cerr << "Sound::Play falhou: " << Mix_GetError() << "\n";
  }
}

void Sound::Stop() {
  if (channel != -1) {
    Mix_HaltChannel(channel);
    channel = -1;
  }
}

void Sound::Open(const std::string& file) {
  chunk = Resources::GetSound(file);
  if (!chunk) {
    std::cerr << "Sound::Open falhou para '" << file << "'\n";
  }
}

bool Sound::IsOpen() const {
  return chunk != nullptr;
}
