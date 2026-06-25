#ifndef RESOURCES_H
#define RESOURCES_H

#include <string>
#include <unordered_map>


struct SDL_Texture;
typedef struct _Mix_Music Mix_Music;
typedef struct Mix_Chunk Mix_Chunk;

class Resources {
public:
  static SDL_Texture* GetImage(const std::string& file);
  static void ClearImages();

  static Mix_Music* GetMusic(const std::string& file);
  static void ClearMusics();

  static Mix_Chunk* GetSound(const std::string& file);
  static void ClearSounds();

private:
  static std::unordered_map<std::string, SDL_Texture*> imageTable;
  static std::unordered_map<std::string, Mix_Music*>   musicTable;
  static std::unordered_map<std::string, Mix_Chunk*>   soundTable;
};

#endif
