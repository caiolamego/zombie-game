#include "TileSet.h"

TileSet::TileSet(int tileWidth, int tileHeight, std::string file) 
    : tileSet(file), tileWidth(tileWidth), tileHeight(tileHeight) {
    // Calcula as colunas baseando-se no tamanho total da imagem
    columns = tileSet.GetWidth() / tileWidth;
    rows = tileSet.GetHeight() / tileHeight;
    tileSet.SetFrameCount(columns, rows);
}

void TileSet::RenderTile(unsigned index, float x, float y) {
    if (index < (unsigned)(rows * columns)) {
        tileSet.SetFrame(index);
        tileSet.Render(x, y);
    }
}

int TileSet::GetTileWidth() { return tileWidth; }
int TileSet::GetTileHeight() { return tileHeight; }