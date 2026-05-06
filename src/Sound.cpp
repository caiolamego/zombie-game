#include "Sound.h"
#include "Resources.h"

Sound::Sound() : chunk(nullptr), channel(-1) {}

Sound::Sound(std::string file) : Sound() {
    Open(file);
}

Sound::~Sound() {
    if (chunk != nullptr) {
        Stop(); // NÃO chama FreeChunk. O Resources cuida disso.
    }
}

void Sound::Play(int times) {
    if (chunk != nullptr) {
        // A Mixer usa 'loops' (quantas vezes repetir). Se times=1, loops=0 (toca 1 vez).
        channel = Mix_PlayChannel(-1, chunk, times - 1);
    }
}

void Sound::Stop() {
    if (chunk != nullptr && channel != -1) {
        Mix_HaltChannel(channel);
    }
}

void Sound::Open(std::string file) {
    chunk = Resources::GetSound(file);
}

bool Sound::IsOpen() {
    return chunk != nullptr;
}