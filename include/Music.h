#ifndef MUSIC_H
#define MUSIC_H

#include <string>

typedef struct _Mix_Music Mix_Music;

class Music {
public:
  Music();
  explicit Music(const std::string& file);
  ~Music();

  void Play(int times = -1);
  void Stop(int msToStop = 1500);
  void Open(const std::string& file);
  bool IsOpen() const;

private:
  Mix_Music* music;
};

#endif 
