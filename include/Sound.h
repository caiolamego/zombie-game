#ifndef SOUND_H
#define SOUND_H

#include <string>

typedef struct Mix_Chunk Mix_Chunk;

class Sound {
public:
  Sound();
  explicit Sound(const std::string& file);
  ~Sound();

  void Play(int times = 1);
  void Stop();
  void Open(const std::string& file);
  bool IsOpen() const;

private:
  Mix_Chunk* chunk;
  int channel; 
};

#endif
