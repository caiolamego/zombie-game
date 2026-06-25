#define INCLUDE_SDL_MIXER
#include "SDL_include.h"

#include "Music.h"
#include "Resources.h"
#include <iostream>

Music::Music() : music(nullptr) {}

Music::Music(const std::string& file) : music(nullptr) {
  Open(file);
}

Music::~Music() {
  Stop(0);
  music = nullptr;
}

void Music::Play(int times) {
  if (music) {
    if (Mix_PlayMusic(music, times) != 0) {
      std::cerr << "Mix_PlayMusic: " << Mix_GetError() << "\n";
    }
  }
}

void Music::Stop(int msToStop) {
  if (Mix_PlayingMusic()) {
    if (Mix_FadeOutMusic(msToStop) == 0) {
      Mix_HaltMusic();
    }
  }
}

void Music::Open(const std::string& file) {
  music = Resources::GetMusic(file);
  if (!music) {
    std::cerr << "Music::Open falhou (arquivo: " << file << ")\n";
  }
}

bool Music::IsOpen() const {
  return music != nullptr;
}
