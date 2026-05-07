#include "TileMap.h"
#include "GameObject.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>
#include "Camera.h"

TileMap::TileMap(GameObject& associated, std::string file, TileSet* tileSet) 
    : Component(associated) {
    SetTileSet(tileSet);
    Load(file);
}

void TileMap::SetTileSet(TileSet* tileSet) {
    this->tileSet.reset(tileSet); 
}

void TileMap::Load(std::string file) {
    tileMatrix.clear();
    mapWidth = mapHeight = mapDepth = 0;

    std::ifstream fin(file);
    if (!fin) {
        std::cout << "TileMap: nao foi possivel abrir '" << file << "'\n";
        return;
    }

    // Lê tudo para a memória e limpa a sujeira do Windows
    std::string content((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());
    std::replace(content.begin(), content.end(), ',', ' ');
    std::stringstream ss(content);

    ss >> mapWidth >> mapHeight >> mapDepth;
    if (!ss || mapWidth <= 0 || mapHeight <= 0 || mapDepth <= 0) {
        std::cout << "TileMap: cabecalho invalido." << std::endl;
        mapWidth = mapHeight = mapDepth = 0;
        return;
    }

    const int total = mapWidth * mapHeight * mapDepth;
    tileMatrix.reserve(total);

    int v;
    while (ss >> v) {
        tileMatrix.push_back(v);
    }

    if ((int)tileMatrix.size() < total) {
        tileMatrix.resize(total, -1);
    } else if ((int)tileMatrix.size() > total) {
        tileMatrix.resize(total);
    }
}

int& TileMap::At(int x, int y, int z) {
    static int dummy = -1;
    if (x < 0 || y < 0 || z < 0 || x >= mapWidth || y >= mapHeight || z >= mapDepth) return dummy;
    return tileMatrix[x + (y * mapWidth) + (z * mapWidth * mapHeight)];
}

void TileMap::RenderLayer(int layer) {
    if (!tileSet || layer < 0 || layer >= mapDepth) return;
    int tw = tileSet->GetTileWidth();
    int th = tileSet->GetTileHeight();

    float parallaxFactor = 1.0f + (layer * 0.5f); 

    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            int index = At(x, y, layer);
            if (index >= 0) { 
                float sx = associated.box.x + x * tw - (Camera::GetInstance().pos.x * parallaxFactor);
                float sy = associated.box.y + y * th - (Camera::GetInstance().pos.y * parallaxFactor);
                tileSet->RenderTile((unsigned)index, sx, sy);
            }
        }
    }
}

void TileMap::Render() {
    for (int z = 0; z < mapDepth; ++z) {
        RenderLayer(z);
    }
}

void TileMap::Update(float dt) { }

int TileMap::GetWidth() { return mapWidth; }
int TileMap::GetHeight() { return mapHeight; }
int TileMap::GetDepth() { return mapDepth; }