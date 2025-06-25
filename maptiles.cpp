/**
 * @file maptiles.cpp
 * Code for the maptiles function.
 */

#include <iostream>
#include <map>

#include "maptiles.h"

using namespace std;


Point<3> convertToXYZ(LUVAPixel pixel) {
    return Point<3>( pixel.l, pixel.u, pixel.v );
}
//helper to measure color diff
double colorDiff(const LUVAPixel& color1, const LUVAPixel& color2) {
    double L = color1.l - color2.l;
    double U = color1.u - color2.u;
    double V = color1.v - color2.v;
    return sqrt(L * L + U * U + V * V);
}
//helper to find closest tile
TileImage* nearbyTile(const LUVAPixel& part, vector<TileImage>& theTiles) {
    TileImage* nearby = nullptr;
    double minDist = std::numeric_limits<double>::max();

    for (TileImage& tile : theTiles) {
        double dist = colorDiff(tile.getAverageColor(), part);
        if (dist < minDist) {
            minDist = dist;
            nearby = &tile;
        }
    }

    return nearby;
}

//helper that changes tile on canvas
void changeTile(MosaicCanvas* canvas, int row, int col, TileImage* tile) {
    if (tile != nullptr) {
        canvas->setTile(row, col, tile);
    }
}

MosaicCanvas* mapTiles(SourceImage const& theSource,
                       vector<TileImage>& theTiles)
{
    int rows = theSource.getRows();
    int cols = theSource.getColumns();

    MosaicCanvas* canvas = new MosaicCanvas(rows, cols);

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            LUVAPixel region = theSource.getRegionColor(row, col);
            TileImage* closestTile = nearbyTile(region, theTiles);
            changeTile(canvas, row, col, closestTile);
        }
    }

    return canvas;
}


