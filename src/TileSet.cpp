#include "TileSet.h"

TileSet::TileSet(int tileWidth, int tileHeight, std::string file) 
    : tileSet(file), tileWidth(tileWidth), tileHeight(tileHeight) {
    // Calcula quantas colunas e linhas a imagem inteira possui com base no tamanho do Tile
    columns = tileSet.GetWidth() / tileWidth;
    rows = tileSet.GetHeight() / tileHeight;
    tileSet.SetFrameCount(columns, rows); // Ensina o Sprite a fatiar a imagem
}

void TileSet::RenderTile(unsigned index, float x, float y) {
    if (index < (unsigned)(rows * columns)) { // Checa se o index é válido
        tileSet.SetFrame(index); // Corta o tile certo
        tileSet.Render(x, y); // Desenha
    }
}

int TileSet::GetTileWidth() { return tileWidth; }
int TileSet::GetTileHeight() { return tileHeight; }