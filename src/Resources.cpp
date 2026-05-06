#include "Resources.h"
#include "Game.h"
#include <iostream>

// Inicialização das tabelas estáticas
std::unordered_map<std::string, SDL_Texture*> Resources::imageTable;
std::unordered_map<std::string, Mix_Music*> Resources::musicTable;
std::unordered_map<std::string, Mix_Chunk*> Resources::soundTable;

SDL_Texture* Resources::GetImage(std::string file) {
    auto it = imageTable.find(file);
    if (it != imageTable.end()) {
        return it->second; // Se já existe na memória, retorna o ponteiro
    }
    
    // Se não existe, carrega na memória
    SDL_Texture* texture = IMG_LoadTexture(Game::GetInstance().GetRenderer(), file.c_str());
    if (texture == nullptr) {
        std::cout << "Erro ao carregar imagem: " << file << " - " << SDL_GetError() << std::endl;
    }
    imageTable[file] = texture; // Salva na tabela
    return texture;
}

void Resources::ClearImages() {
    for (auto& par : imageTable) {
        SDL_DestroyTexture(par.second); // Desaloca da VRAM
    }
    imageTable.clear();
}

Mix_Music* Resources::GetMusic(std::string file) {
    auto it = musicTable.find(file);
    if (it != musicTable.end()) {
        return it->second;
    }
    
    Mix_Music* music = Mix_LoadMUS(file.c_str());
    if (music == nullptr) {
        std::cout << "Erro ao carregar música: " << file << " - " << SDL_GetError() << std::endl;
    }
    musicTable[file] = music;
    return music;
}

void Resources::ClearMusics() {
    for (auto& par : musicTable) {
        Mix_FreeMusic(par.second);
    }
    musicTable.clear();
}

Mix_Chunk* Resources::GetSound(std::string file) {
    auto it = soundTable.find(file);
    if (it != soundTable.end()) {
        return it->second;
    }
    
    Mix_Chunk* chunk = Mix_LoadWAV(file.c_str());
    if (chunk == nullptr) {
        std::cout << "Erro ao carregar som: " << file << " - " << SDL_GetError() << std::endl;
    }
    soundTable[file] = chunk;
    return chunk;
}

void Resources::ClearSounds() {
    for (auto& par : soundTable) {
        Mix_FreeChunk(par.second);
    }
    soundTable.clear();
}