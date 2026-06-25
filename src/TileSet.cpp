#include "TileSet.h"
#include "Sprite.h"
#include <iostream>

TileSet::TileSet(int tw, int th, const std::string& file)
: tileSet(file, 1, 1), tileWidth(tw), tileHeight(th), tileCount(0) {
  if (!tileSet.IsOpen()) {
    std::cerr << "TileSet: falha ao abrir imagem '" << file << "'\n";
    return;
  }

  const int sheetW = tileSet.GetWidth();
  const int sheetH = tileSet.GetHeight();

  if (tileWidth <= 0 || tileHeight <= 0) {
    std::cerr << "TileSet: dimensoes invalidas dos tiles." << std::endl;
    return;
  }

  const int cols = sheetW / tileWidth;
  const int rows = sheetH / tileHeight;

  if (cols <= 0 || rows <= 0) {
    std::cerr << "TileSet: calculo de grid invalido (cols/rows)." << std::endl;
    return;
  }

  const_cast<Sprite&>(tileSet).SetFrameCount(cols, rows);
  tileCount = cols * rows;
}

void TileSet::RenderTile(unsigned index, float x, float y) const {
  if (!tileSet.IsOpen() || tileCount <= 0) return;
  if (index >= (unsigned)tileCount) return; 

  const_cast<Sprite&>(tileSet).SetFrame((int)index);
  const_cast<Sprite&>(tileSet).Render((int)x, (int)y, tileWidth, tileHeight);
}
