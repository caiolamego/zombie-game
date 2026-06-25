#pragma once
#define INCLUDE_SDL
#include "SDL_include.h"
#include <string>
#include "Vec2.h"

class Sprite {
private:
    SDL_Texture* texture;
    int width;
    int height;
    SDL_Rect clipRect;
    
    int frameCountW;
    int frameCountH; 
    Vec2 scale;
    SDL_RendererFlip flip;

public:
    Sprite();
    Sprite(std::string file, int frameCountW = 1, int frameCountH = 1); 
    ~Sprite();
    
    void Open(std::string file);
    void SetClip(int x, int y, int w, int h);
    void Render(int x, int y, float angle = 0.0f);
    
    int GetWidth();
    int GetHeight();
    bool IsOpen();
    
    void SetFrame(int frame);
    void SetFrameCount(int frameCountW, int frameCountH);

    void SetScale(float scaleX, float scaleY);
    Vec2 GetScale() const;
    void SetFlip(bool flipH, bool flipV);
};