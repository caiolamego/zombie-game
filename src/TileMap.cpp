#include "TileMap.h"
#include "GameObject.h"
#include "Camera.h" 
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>

TileMap::TileMap(GameObject& associated, const std::string& file, TileSet* ts)
: Component(associated), tileSet(nullptr) {
  SetTileSet(ts);
  Load(file);
}

void TileMap::SetTileSet(TileSet* ts) {
  tileSet.reset(ts); 
}

void TileMap::Load(const std::string& file) {
  tileMatrix.clear();
  mapWidth = mapHeight = mapDepth = 0;

  std::ifstream fin(file);
  if (!fin) {
    std::cerr << "TileMap: nao foi possivel abrir '" << file << "'\n";
    return;
  }

  std::string content((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());
  std::replace(content.begin(), content.end(), ',', ' ');
  std::stringstream ss(content);

  ss >> mapWidth >> mapHeight >> mapDepth;
  if (!ss || mapWidth <= 0 || mapHeight <= 0 || mapDepth <= 0) {
    std::cerr << "TileMap: cabecalho invalido (W H D)." << std::endl;
    mapWidth = mapHeight = mapDepth = 0;
    return;
  }

  const int total = mapWidth * mapHeight * mapDepth;
  tileMatrix.reserve(total);

  int v;
  while (ss >> v) tileMatrix.push_back(v);

  if ((int)tileMatrix.size() < total) {
    std::cerr << "TileMap: valores insuficientes no arquivo (esperado " << total << ")\n";
    tileMatrix.resize(total, -1);
  } else if ((int)tileMatrix.size() > total) {
    tileMatrix.resize(total);
  }
}

int& TileMap::At(int x, int y, int z) {
  static int dummy = -1;
  if (x < 0 || y < 0 || z < 0 || x >= mapWidth || y >= mapHeight || z >= mapDepth) return dummy;
  return tileMatrix[Index(x,y,z)];
}

void TileMap::RenderLayer(int layer) {
  if (!tileSet || layer < 0 || layer >= mapDepth) return;

  const int tw = tileSet->GetTileWidth();
  const int th = tileSet->GetTileHeight();

  const Vec2 cam = Camera::GetInstance().pos;

  for (int y = 0; y < mapHeight; ++y) {
    for (int x = 0; x < mapWidth; ++x) {
      int idx = tileMatrix[Index(x,y,layer)];
      if (idx >= 0) {
        float sx = (associated.box.x + x * tw) - cam.x;
        float sy = (associated.box.y + y * th) - cam.y;
        tileSet->RenderTile((unsigned)idx, sx, sy);
      }
    }
  }
}

void TileMap::Render() {
  for (int z = 0; z < mapDepth; ++z) RenderLayer(z);
}

