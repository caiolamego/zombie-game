#ifndef TILESET_H
#define TILESET_H

#include <string>
#include "Sprite.h"

class TileSet {
public:
  TileSet(int tileWidth, int tileHeight, const std::string& file);

  void RenderTile(unsigned index, float x, float y) const;

  int GetTileWidth()  const { return tileWidth; }
  int GetTileHeight() const { return tileHeight; }
  int GetTileCount()  const { return tileCount; }

private:
  Sprite tileSet;      
  int tileWidth;       
  int tileHeight;      
  int tileCount;       
};

#endif
