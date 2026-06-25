#ifndef TILEMAP_H
#define TILEMAP_H

#include <vector>
#include <memory>
#include <string>
#include "Component.h"
#include "TileSet.h"

class TileMap : public Component {
public:
  TileMap(GameObject& associated, const std::string& file, TileSet* tileSet);

  void Load(const std::string& file);
  void SetTileSet(TileSet* ts);

  int& At(int x, int y, int z = 0);

  void Render() override;
  void RenderLayer(int layer);
  void Update(float) override {}

  int GetWidth()  const { return mapWidth; }
  int GetHeight() const { return mapHeight; }
  int GetDepth()  const { return mapDepth; }

private:
  std::vector<int> tileMatrix;           
  std::unique_ptr<TileSet> tileSet;      
  int mapWidth  = 0;                     
  int mapHeight = 0;                     
  int mapDepth  = 0;                     

  inline int Index(int x, int y, int z) const {
    return z * (mapWidth * mapHeight) + y * mapWidth + x;
  }
};

#endif
