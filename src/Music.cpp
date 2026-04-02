#include "Music.h"
#include <iostream>

Music::Music() {
    music = nullptr; // Inicializa music como nullptr [cite: 268]
}

Music::Music(std::string file) {
    music = nullptr;
    Open(file); // O segundo construtor chama Open [cite: 268]
}

Music::~Music() {
    Stop(0);
    if (music != nullptr) {
        Mix_FreeMusic(music); // Libera a música da memória [cite: 284, 285]
    }
}

void Music::Play(int times) {
    if (music != nullptr) {
        Mix_PlayMusic(music, times); // Recebe uma música e quantas vezes ela deve ser tocada [cite: 271]
    }
}

void Music::Stop(int msToStop) {
    Mix_FadeOutMusic(msToStop); // Para a música atual dando um efeito de fade [cite: 276]
}

void Music::Open(std::string file) {
    music = Mix_LoadMUS(file.c_str()); // Carrega a música indicada no arquivo [cite: 280]
    if (music == nullptr) {
        std::cout << "Erro ao carregar musica: " << SDL_GetError() << std::endl;
    }
}

bool Music::IsOpen() {
    return music != nullptr; // Checa se music é nula [cite: 282]
}