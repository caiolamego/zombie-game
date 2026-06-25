#define INCLUDE_SDL
#define INCLUDE_SDL_IMAGE
#define INCLUDE_SDL_MIXER
#include "SDL_include.h"

#include "Resources.h"
#include "Game.h"
#include <iostream>

std::unordered_map<std::string, SDL_Texture*> Resources::imageTable;
std::unordered_map<std::string, Mix_Music*>   Resources::musicTable;
std::unordered_map<std::string, Mix_Chunk*>   Resources::soundTable;

SDL_Texture* Resources::GetImage(const std::string& file) {
  auto it = imageTable.find(file);
  if (it != imageTable.end()) return it->second;

  SDL_Renderer* r = Game::GetInstance().GetRenderer();
  SDL_Texture* t = IMG_LoadTexture(r, file.c_str());
  if (!t) {
    std::cerr << "Resources::GetImage falhou: " << IMG_GetError() << " (" << file << ")\n";
    return nullptr;
  }
  imageTable[file] = t;
  return t;
}

void Resources::ClearImages() {
  for (auto& kv : imageTable) {
    if (kv.second) SDL_DestroyTexture(kv.second);
  }
  imageTable.clear();
}

Mix_Music* Resources::GetMusic(const std::string& file) {
  auto it = musicTable.find(file);
  if (it != musicTable.end()) return it->second;

  Mix_Music* m = Mix_LoadMUS(file.c_str());
  if (!m) {
    std::cerr << "Resources::GetMusic falhou: " << Mix_GetError() << " (" << file << ")\n";
    return nullptr;
  }
  musicTable[file] = m;
  return m;
}

void Resources::ClearMusics() {
  for (auto& kv : musicTable) {
    if (kv.second) Mix_FreeMusic(kv.second);
  }
  musicTable.clear();
}

Mix_Chunk* Resources::GetSound(const std::string& file) {
  auto it = soundTable.find(file);
  if (it != soundTable.end()) return it->second;

  Mix_Chunk* c = Mix_LoadWAV(file.c_str());
  if (!c) {
    std::cerr << "Resources::GetSound falhou: " << Mix_GetError() << " (" << file << ")\n";
    return nullptr;
  }
  soundTable[file] = c;
  return c;
}

void Resources::ClearSounds() {
  for (auto& kv : soundTable) {
    if (kv.second) Mix_FreeChunk(kv.second);
  }
  soundTable.clear();
}
