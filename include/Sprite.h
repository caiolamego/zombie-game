#pragma once
#include <SDL2/SDL.h>
#include <string>

class Sprite {
private:
    SDL_Texture* texture;
    int width;
    int height;
    SDL_Rect clipRect;
    
    int frameCountW;
    int frameCountH; 

public:
    Sprite();
    Sprite(std::string file, int frameCountW = 1, int frameCountH = 1); 
    ~Sprite();
    
    void Open(std::string file);
    void SetClip(int x, int y, int w, int h);
    void Render(int x, int y);
    
    int GetWidth();
    int GetHeight();
    bool IsOpen();
    
    void SetFrame(int frame);
    void SetFrameCount(int frameCountW, int frameCountH);
};