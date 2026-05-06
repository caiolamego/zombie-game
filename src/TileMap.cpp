#include "TileMap.h"
#include "GameObject.h"
#include <fstream>
#include <sstream>
#include <algorithm> // Necessário para a mágica do std::replace do projeto SS
#include <iostream>

TileMap::TileMap(GameObject& associated, std::string file, TileSet* tileSet) 
    : Component(associated) {
    SetTileSet(tileSet);
    Load(file);
}

void TileMap::SetTileSet(TileSet* tileSet) {
    this->tileSet.reset(tileSet); 
}

// O Leitor Blindado Baseado no Projeto SS
void TileMap::Load(std::string file) {
    tileMatrix.clear();
    mapWidth = mapHeight = mapDepth = 0;

    std::ifstream fin(file);
    if (!fin) {
        std::cout << "TileMap: nao foi possivel abrir '" << file << "'\n";
        return;
    }

    // Lê o arquivo inteiro para a memória de uma vez só
    std::string content((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());
    
    // Substitui absolutamente todas as vírgulas por espaços
    std::replace(content.begin(), content.end(), ',', ' ');
    
    // Passa a string limpa para o extrator
    std::stringstream ss(content);

    // O extrator lê as 3 primeiras variáveis pulando qualquer lixo invisível
    ss >> mapWidth >> mapHeight >> mapDepth;
    
    if (!ss || mapWidth <= 0 || mapHeight <= 0 || mapDepth <= 0) {
        std::cout << "TileMap: cabecalho invalido." << std::endl;
        mapWidth = mapHeight = mapDepth = 0;
        return;
    }

    // Otimização de memória aprendida no projeto SS
    const int total = mapWidth * mapHeight * mapDepth;
    tileMatrix.reserve(total);

    int v;
    // Laço a prova de balas que não escorrega o array
    while (ss >> v) {
        // Subtraímos 1 para o TileD 0 virar espaço vazio (-1)
        tileMatrix.push_back(v - 1);
    }

    // Failsafe do projeto SS: Se o Tiled exportar faltando peças, preenche com vazio (-1)
    if ((int)tileMatrix.size() < total) {
        tileMatrix.resize(total, -1);
    } else if ((int)tileMatrix.size() > total) {
        tileMatrix.resize(total);
    }
}

int& TileMap::At(int x, int y, int z) {
    // Fórmula de achatamento
    return tileMatrix[x + (y * mapWidth) + (z * mapWidth * mapHeight)];
}

void TileMap::RenderLayer(int layer) {
    // Se a layer pedida for inválida, sai da função
    if (!tileSet || layer < 0 || layer >= mapDepth) return;

    int tw = tileSet->GetTileWidth();
    int th = tileSet->GetTileHeight();

    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            int index = At(x, y, layer);
            
            // Só renderiza se for um tile válido (maior ou igual a 0)
            if (index >= 0) { 
                float sx = associated.box.x + x * tw;
                float sy = associated.box.y + y * th;
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